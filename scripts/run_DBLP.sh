cd ../build

current_datetime=$(date +%Y-%m-%d-%H-%M)
mkdir ../logs/$current_datetime
touch ../logs/$current_datetime/logs.txt
EPS=0.5
NUM_CAND_EDGES=0
K_EDGES=50
SEED_MODE="IM"
SEED_MODE="RAND"
SEED_MODE="OUTDEG"
# inf - k
for RAND_SEED in 2021 2022 2023 2024 2025;
do
    for K_EDGES in 5 10 20 30 40 50;
    do
    ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE
    done
done

# # inf - eps
# for RAND_SEED in 2021 2022 2023 2024 2025;
# do
#     for EPS in 0.1 0.2 0.3 0.4;
#     do
#     ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE
#     done
# done

# EPS=0.5
for RAND_SEED in 2021 2022 2023 2024 2025;
do
    for BETA in 2 4 8 16 32 64;
    do
    ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
    done
done

# RAND_SEED=2022
# BETA=2
# ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# BETA=4
# ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# BETA=8
# ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# BETA=16
# ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# BETA=32
# ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# BETA=64
# ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA

# RAND_SEED=2023
# BETA=2
# ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# BETA=4
# ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# BETA=8
# ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# BETA=16
# ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# BETA=32
# ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# BETA=64
# ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA

# RAND_SEED=2024
# BETA=2
# ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# BETA=4
# ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# BETA=8
# ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# BETA=16
# ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# BETA=32
# ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# BETA=64
# ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA

# ./IMA_only -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed 2023 -num_cand_edges $NUM_CAND_EDGES -probability WC -seed_mode IM

# ./eval_by_mc -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed 2023 -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode IM -edges_mode IMA -beta 2 -log_step 5
# ./eval_by_mc -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed 2023 -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode IM -edges_mode IMA -beta 4 -log_step 5
# ./eval_by_mc -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed 2023 -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode IM -edges_mode IMA -beta 8 -log_step 5
# ./eval_by_mc -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed 2023 -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode IM -edges_mode IMA -beta 16 -log_step 5
# ./eval_by_mc -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed 2023 -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode IM -edges_mode IMA -beta 32 -log_step 5
# ./eval_by_mc -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed 2023 -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode IM -edges_mode IMA -beta 64 -log_step 5
# ./eval_by_mc -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed 2023 -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode IM -edges_mode UINF -log_step 5
# ./eval_by_mc -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed 2023 -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode IM -edges_mode PROB -log_step 5
# ./eval_by_mc -dataset ../data/DBLP -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed 2023 -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode IM -edges_mode OUTDEG -log_step 5


# ./IMA_only -dataset ../data/DBLP -epsilon 0.5 -delta 0.001 -k_edges 50 -rand_seed 2023 -num_cand_edges 0 -fast True -probability WC -seed_mode IM | tee ../logs/$current_datetime/logs.txt
# ./eval_by_mc -dataset ../data/DBLP -epsilon 0.5 -delta 0.001 -k_edges 50 -rand_seed 2023 -num_cand_edges 0 -fast True -probability WC -seed_mode IM -edges_mode IMA -log_step 5
# ./eval_by_mc -dataset ../data/DBLP -epsilon 0.5 -delta 0.001 -k_edges 50 -rand_seed 2023 -num_cand_edges 0 -fast True -probability WC -seed_mode IM -edges_mode RAND -log_step 5
# ./eval_by_mc -dataset ../data/DBLP -epsilon 0.5 -delta 0.001 -k_edges 50 -rand_seed 2023 -num_cand_edges 0 -fast True -probability WC -seed_mode IM -edges_mode OUTDEG -log_step 5
# ./eval_by_mc -dataset ../data/DBLP -epsilon 0.5 -delta 0.001 -k_edges 50 -rand_seed 2023 -num_cand_edges 0 -fast True -probability WC -seed_mode IM -edges_mode PROB -log_step 5