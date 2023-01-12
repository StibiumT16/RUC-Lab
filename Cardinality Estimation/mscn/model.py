import torch
import torch.nn as nn
import torch.nn.functional as F


# Define model architecture

class SetConv(nn.Module):
    def __init__(self, predicate_feats, join_feats, hid_units):
        super(SetConv, self).__init__()
        self.predicate_mlp1 = nn.Linear(predicate_feats, 2 * hid_units)
        self.predicate_mlp2 = nn.Linear(2 * hid_units, hid_units)
        self.predicate_mlp3 = nn.Linear(hid_units, hid_units)
        self.join_mlp1 = nn.Linear(join_feats, 2 * hid_units)
        self.join_mlp2 = nn.Linear(2 * hid_units, hid_units)
        self.join_mlp3 = nn.Linear(hid_units, hid_units)
        self.out_mlp1 = nn.Linear(hid_units * 2, hid_units)
        self.out_mlp2 = nn.Linear(hid_units, hid_units)
        self.out_mlp3 = nn.Linear(hid_units, 1)
        self.dropout = nn.Dropout(p=0.3)
        

    # def forward(self, samples, predicates, joins, sample_mask, predicate_mask, join_mask):
    def forward(self, predicates, joins, predicate_mask, join_mask):
        # samples has shape [batch_size x num_joins+1 x sample_feats]
        # predicates has shape [batch_size x num_predicates x predicate_feats]
        # joins has shape [batch_size x num_joins x join_feats]

        hid_predicate = F.relu(self.predicate_mlp1(predicates))
        hid_predicate = F.relu(self.predicate_mlp2(hid_predicate))
        hid_predicate = F.relu(self.predicate_mlp3(hid_predicate))
        hid_predicate = self.dropout(hid_predicate)
        hid_predicate = hid_predicate * predicate_mask
        hid_predicate = torch.sum(hid_predicate, dim=1, keepdim=False)
        predicate_norm = predicate_mask.sum(1, keepdim=False)
        hid_predicate = hid_predicate / predicate_norm

        hid_join = F.relu(self.join_mlp1(joins))
        hid_join = F.relu(self.join_mlp2(hid_join))
        hid_join = F.relu(self.join_mlp3(hid_join))
        hid_join  = self.dropout(hid_join)
        hid_join = hid_join * join_mask
        hid_join = torch.sum(hid_join, dim=1, keepdim=False)
        join_norm = join_mask.sum(1, keepdim=False)
        hid_join = hid_join / join_norm

        hid = torch.cat((hid_predicate, hid_join), 1)
        hid = F.relu(self.out_mlp1(hid))
        hid = F.relu(self.out_mlp2(hid))
        hid = self.dropout(hid)
        out = torch.sigmoid(self.out_mlp3(hid))
        return out
