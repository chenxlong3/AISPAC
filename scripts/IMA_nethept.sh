cd ../build

# current_datetime=$(date +%Y-%m-%d-%H-%M)
# mkdir ../logs/$current_datetime
# touch ../logs/$current_datetime/logs.txt
EPS=0.5
NUM_CAND_EDGES=0
K_EDGES=50
RAND_SEED=2021
NUM_CAND_EDGES=10000
SEED_MODE="IM"
# SEED_MODE="OUTDEG"
# SEED_MODE="RAND"
# for RAND_SEED in 42 2020 2022 2023 2024;
# do
#     for K_EDGES in 5 10 20 30 40 50;
#     do
#     ./IMA_only -dataset ../data/nethept -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE
#     done
# done

# SEED_MODE="RAND"
# for RAND_SEED in 42 2020 2022 2023 2024;
# do
#     for K_EDGES in 5 10 20 30 40 50;
#     do
#     ./IMA_only -dataset ../data/nethept -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE
#     done
# done


# # Influence - k
# for RAND_SEED in 42 2020 2022 2023 2024;
# do
#     for K_EDGES in 5 10 20 30 40 50;
#     do
#     ./IMA_only -dataset ../data/nethept -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE
#     done
# done

# # Influence - eps
# K_EDGES=50
# for RAND_SEED in 42 2020 2022 2023 2024;
# do
#     for EPS in 0.1 0.2 0.3 0.4;
#     do
#     ./IMA_only -dataset ../data/nethept -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE
#     done
# done

# Inf - beta
# EPS=0.5
# for RAND_SEED in 42 2020 2022 2023 2024;
# do
#     for BETA in 2 4 8 16 32 64;
#     do
#     ./IMA_only -dataset ../data/nethept -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
#     done 
# done


# ./eval_by_mc -dataset ../data/nethept -epsilon 0.5 -delta 0.001 -k_edges 50 -rand_seed 2023 -num_cand_edges 0 -fast True -probability WC -seed_mode $SEED_MODE -edges_mode IMA -log_step 5
# ./eval_by_mc -dataset ../data/nethept -epsilon 0.5 -delta 0.001 -k_edges 50 -rand_seed 2023 -num_cand_edges 0 -fast True -probability WC -seed_mode $SEED_MODE -edges_mode RAND -log_step 5
# ./eval_by_mc -dataset ../data/nethept -epsilon 0.5 -delta 0.001 -k_edges 50 -rand_seed 2023 -num_cand_edges 0 -fast True -probability WC -seed_mode $SEED_MODE -edges_mode OUTDEG -log_step 5
# ./eval_by_mc -dataset ../data/nethept -epsilon 0.5 -delta 0.001 -k_edges 50 -rand_seed 2023 -num_cand_edges 0 -fast True -probability WC -seed_mode $SEED_MODE -edges_mode PROB -log_step 5

# current_datetime=$(date +%Y-%m-%d-%H-%M)
# mkdir ../logs/$current_datetime
# touch ../logs/$current_datetime/logs.txt
# ./IMA_only -dataset ../data/nethept -epsilon 0.1 -delta 0.001 -k_edges 50 -rand_seed 2023 -fast True -probability WC | tee ../logs/$current_datetime/logs.txt

# Rebuttal
./RW -dataset ../data/nethept -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE