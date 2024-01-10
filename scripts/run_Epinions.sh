cd ../build

# current_datetime=$(date +%Y-%m-%d-%H-%M)
# mkdir ../logs/$current_datetime
# touch ../logs/$current_datetime/logs.txt
EPS=0.5
NUM_CAND_EDGES=0
K_EDGES=152
K_SEED=76
SEED_MODE="IM"
SEED_MODE="RAND"
# SEED_MODE="OUTDEG"
# Influence - k
# for RAND_SEED in 2021 2022 2023 2024 2025;
# do
#     for K_EDGES in 5 10 20 30 40 50;
#     do
#     ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE
#     done
# done

# # Influence - eps
# K_EDGES=50
# for RAND_SEED in 2021 2022 2023 2024 2025;
# do
#     for EPS in 0.1 0.2 0.3 0.4;
#     do
#     ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE
#     done
# done

# Inf - beta
# EPS=0.5
# for RAND_SEED in 2021 2022 2023 2024 2025;
# do
#     for BETA in 2 4 8 16 32 64;
#     do
#     ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
#     done
# done


# RAND_SEED=2022
# BETA=2
# ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# BETA=4
# ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# BETA=8
# ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# BETA=16
# ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# BETA=32
# ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# BETA=64
# ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA

RAND_SEED=2021
NUM_MC=500
# for NUM_MC in 1000 1500 2000
for NUM_MC in 25000 30000
do
# ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta 6000
./MCGreedy_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta 6000 -num_mc $NUM_MC &
done
# ./MCGreedy_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta 6000 -num_mc $NUM_MC