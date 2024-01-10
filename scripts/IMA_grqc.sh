cd ../build

EPS=0.5
NUM_CAND_EDGES=10000
K_EDGES=50
RAND_SEED=2021
# current_datetime=$(date +%Y-%m-%d-%H-%M)
# mkdir ../logs/$current_datetime
# touch ../logs/$current_datetime/logs.txt

# ./format_graph -dataset ../data/GRQC -filename edgelist_ic.txt
SEED_MODE="RAND"
# SEED_MODE="OUTDEG"
SEED_MODE="IM"
# for SEED_MODE in "RAND" "OUTDEG" "IM"
# do
#     for RAND_SEED in 2021 2022 2023 2024 2025;
#     do
#     for K_EDGES in 5 10 20 30 40 50
#         do
#         ./IMA_only -dataset ../data/GRQC -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE
#         done
#     done
# done

# SEED_MODE="IM"
# for RAND_SEED in 2021 2022 2023 2024 2025;
# do
# for K_EDGES in 5 10 20 30 40 50
#     do
#     ./IMA_only -dataset ../data/GRQC -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE
#     done
# done

# K_EDGES=5
# ./IMA_only -dataset ../data/GRQC -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed 2023 -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode RAND

./IMA_only -dataset ../data/GRQC -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE