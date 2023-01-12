import os

epochs = 1000
batch = 100
lr = 1e-3
hid = 256
num_queries = 5000
data_name = "synthetic"
save_name=f'syn_{epochs}'


os.system(f"python3 train.py \
        --epochs {epochs} \
        --batch {batch} \
        --lr {lr} \
        --hid {hid} \
        --save_name {save_name} \
        --num_queries {num_queries} \
        --data_name {data_name} \
        --load_ckpt False")

os.system(f"python3 train.py \
        --epochs {epochs} \
        --batch {batch} \
        --lr {lr} \
        --hid {hid} \
        --save_name {save_name} \
        --num_queries {num_queries} \
        --data_name {data_name} \
        --load_ckpt True")
