import argparse
import time
import os
import random
from tqdm import tqdm

import torch
from torch.autograd import Variable
from torch.utils.data import DataLoader
from transformers import AdamW, get_linear_schedule_with_warmup
from sklearn.metrics import mean_squared_log_error

from mscn.util import *
from mscn.data import get_train_datasets, load_data, make_dataset
from mscn.model import SetConv

parser = argparse.ArgumentParser()
parser.add_argument("--epochs", default=100, type=int)
parser.add_argument("--batch", default=1000, type=int)
parser.add_argument("--lr", default=1e-3, type=float)
parser.add_argument("--hid", default=256, type=int)
parser.add_argument("--save_name", default="model", type=str)
parser.add_argument("--num_queries", default=10000, type=int)
parser.add_argument("--data_name", default="train", type=str)
parser.add_argument("--load_ckpt", default="True", type=str)
args = parser.parse_args()

logger = open("log.log", "a")
logger.write("\n")
logger.write(f"start a new running with args: {args}\n")

def set_seed(seed=0):
    random.seed(seed)
    os.environ['PYTHONHASHSEED'] = str(seed)
    np.random.seed(seed)
    torch.manual_seed(seed)
    torch.backends.cudnn.deterministic = True


def unnormalize_torch(vals, min_val, max_val):
    vals = (vals * (max_val - min_val)) + min_val
    return torch.exp(vals)


def qerror_loss(preds, targets, min_val, max_val):
    qerror = []
    preds = unnormalize_torch(preds, min_val, max_val)
    targets = unnormalize_torch(targets, min_val, max_val)

    for i in range(len(targets)):
        if (preds[i] > targets[i]).cpu().data.numpy()[0]:
            qerror.append(preds[i] / targets[i])
        else:
            qerror.append(targets[i] / preds[i])
    return torch.mean(torch.cat(qerror))


def predict(model, data_loader):
    preds = []
    model.eval()
    for batch_idx, data_batch in enumerate(data_loader):

        samples, predicates, joins, targets, sample_masks, predicate_masks, join_masks = data_batch
        samples, predicates, joins, targets = Variable(samples), Variable(predicates), Variable(joins), Variable(targets)
        sample_masks, predicate_masks, join_masks = Variable(sample_masks), Variable(predicate_masks), Variable(join_masks)
        outputs = model(predicates, joins, predicate_masks, join_masks)
        for i in range(outputs.data.shape[0]):
            preds.append(outputs.data[i])
    return preds


def print_qerror(preds_unnorm, labels_unnorm):
    qerror = []
    for i in range(len(preds_unnorm)):
        if preds_unnorm[i] > float(labels_unnorm[i]):
            qerror.append(preds_unnorm[i] / float(labels_unnorm[i]))
        else:
            qerror.append(float(labels_unnorm[i]) / float(preds_unnorm[i]))

    print("Median: {}".format(np.median(qerror)))
    print("90th percentile: {}".format(np.percentile(qerror, 90)))
    print("95th percentile: {}".format(np.percentile(qerror, 95)))
    print("99th percentile: {}".format(np.percentile(qerror, 99)))
    print("Max: {}".format(np.max(qerror)))
    print("Mean: {}".format(np.mean(qerror)))


def train_and_predict(num_epochs, batch_size, lr, hid_units, save_name, data_name, num_queries,load_ckpt):
    # Load training and validation data
    dicts, column_min_max_vals, min_val, max_val, labels_train, labels_test, max_num_joins, max_num_predicates, train_data, test_data = get_train_datasets(num_queries, data_name)
    table2vec, column2vec, op2vec, join2vec = dicts

    # Train model
    def train():
        predicate_feats = len(column2vec) + len(op2vec) + 1
        join_feats = len(join2vec)

        model = SetConv(predicate_feats, join_feats, hid_units)

        optimizer = AdamW(model.parameters(), lr=lr, no_deprecation_warning=True)
        t_total = int(num_queries * 0.9 * num_epochs // batch_size)
        scheduler = get_linear_schedule_with_warmup(optimizer, num_warmup_steps=0, num_training_steps=t_total)
    
        train_data_loader = DataLoader(train_data, batch_size=batch_size)
        test_data_loader = DataLoader(test_data, batch_size=batch_size)

        model.train()
        for epoch in range(num_epochs):
            loss_total = 0.

            for batch_idx, data_batch in tqdm(enumerate(train_data_loader)):
                samples, predicates, joins, targets, sample_masks, predicate_masks, join_masks = data_batch
                samples, predicates, joins, targets = Variable(samples), Variable(predicates), Variable(joins), Variable(targets)
                sample_masks, predicate_masks, join_masks = Variable(sample_masks), Variable(predicate_masks), Variable(join_masks)

                optimizer.zero_grad()
                outputs = model(predicates, joins, predicate_masks, join_masks)
                loss = qerror_loss(outputs, targets.float(), min_val, max_val)
                loss_total += loss.item()
                loss.backward()
                torch.nn.utils.clip_grad_norm_(model.parameters(), 5.0)
                optimizer.step()
                scheduler.step()
                for param_group in optimizer.param_groups:
                    learning_rate = param_group['lr']
            print("Epoch {}, loss: {}, lr:{}".format(epoch, loss_total / len(train_data_loader), learning_rate))
            if (epoch + 1) % 10 == 0: 
                logger.write(f"Epoch {epoch}, loss: {loss_total / len(train_data_loader)}, lr:{learning_rate}\n")

        torch.save(model, f"outputs/{save_name}.pkl")
        # Get final training and validation set predictions
        preds_train = predict(model, train_data_loader)
        preds_test = predict(model, test_data_loader)

        # Unnormalize
        preds_train_unnorm = unnormalize_labels(preds_train, min_val, max_val)
        labels_train_unnorm = unnormalize_labels(labels_train, min_val, max_val)

        preds_test_unnorm = unnormalize_labels(preds_test, min_val, max_val)
        labels_test_unnorm = unnormalize_labels(labels_test, min_val, max_val)

        # Print metrics
        print("\nQ-Error training set:")
        print_qerror(preds_train_unnorm, labels_train_unnorm)

        print("\nQ-Error validation set:")
        print_qerror(preds_test_unnorm, labels_test_unnorm)
        print("")
	
    def eval():
        # Load test data
        model = torch.load(f"outputs/{save_name}.pkl")
        file_name = "ce_dataset/test_without_label" 
        joins, predicates, tables, label = load_data(file_name)

        # Get feature encoding and proper normalization
        samples_test = encode_samples(tables, table2vec)
        predicates_test, joins_test = encode_data(predicates, joins, column_min_max_vals, column2vec, op2vec, join2vec)
        labels_test, _, _ = normalize_labels(label, min_val, max_val)

        print("Number of test samples: {}".format(len(labels_test)))

        max_num_predicates = max([len(p) for p in predicates_test])
        max_num_joins = max([len(j) for j in joins_test])

        # Get test set predictions
        test_data = make_dataset(samples_test, predicates_test, joins_test, labels_test, max_num_joins, max_num_predicates)
        test_data_loader = DataLoader(test_data, batch_size=batch_size)
        preds_test= predict(model, test_data_loader)

        # Unnormalize
        preds_test_unnorm = unnormalize_labels(preds_test, min_val, max_val)
        
        #eval
        ans = my_judge(preds_test_unnorm)
        print(ans)
        logger.write(f"MSLE:{ans}\n")

        # Write predictions
        file_name = f"results/{save_name}.csv"
        os.makedirs(os.path.dirname(file_name), exist_ok=True)
        with open(file_name, "w") as f:
            f.write("Query ID" + "," + "Predicted Cardinality" + "\n")
            for i in range(len(preds_test_unnorm)):
                f.write(str(i) + "," + str(preds_test_unnorm[i]) + "\n")

    if load_ckpt == "True":
        eval()
    else:
        train()
    logger.close()


def my_judge(pred):
    truth = []
    truth_path = "/home/tyt/Desktop/CE/ce_dataset/truth.csv"
    with open(truth_path, encoding='utf-8') as fin:
        for line in fin:
            truth.append(int(line))
    ans = mean_squared_log_error(pred, truth, squared=False)
    return ans

if __name__ == "__main__":
    set_seed(42)
    train_and_predict(num_epochs=args.epochs, batch_size=args.batch, lr=args.lr, hid_units=args.hid,  
        save_name=args.save_name, num_queries=args.num_queries, data_name=args.data_name, load_ckpt=args.load_ckpt)
    
