cd ../build

# current_datetime=$(date +%Y-%m-%d-%H-%M)
# mkdir ../logs/$current_datetime
# touch ../logs/$current_datetime/logs.txt
# ./MCGreedy_only -dataset ../data/nethept -epsilon 0.5 -delta 0.001 -k_edges 40 -rand_seed 2023 -num_cand_edges 10000 -fast True -probability WC -seed_mode $SEED_MODE -num_mc 5000 | tee ../logs/$current_datetime/logs.txt

EPS=0.5
NUM_CAND_EDGES=10000
K_EDGES=50
RAND_SEED=2023
SEED_MODE="IM"
SEED_MODE="OUTDEG"
SEED_MODE="RAND"
NUM_MC=1200
./MCGreedy_only -dataset ../data/nethept -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -num_mc $NUM_MC

# for RAND_SEED in 2021 2022 2023 2024 2025
# do
#     for NUM_MC in $(seq 1000 1000 5000)
#     do
#     ./MCGreedy_only -dataset ../data/nethept -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -num_mc $NUM_MC
#     done
# done

# ./MCGreedy_only -dataset ../data/nethept -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -num_mc 1000
# ./MCGreedy_only -dataset ../data/nethept -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -num_mc 2000
# ./MCGreedy_only -dataset ../data/nethept -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -num_mc 3000
# ./MCGreedy_only -dataset ../data/nethept -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -num_mc 4000
# ./MCGreedy_only -dataset ../data/nethept -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -num_mc 5000
# ./eval_by_mc -dataset ../data/nethept -epsilon $EPS -delta 0.001 -k_edges 50 -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -edges_mode MCGreedy_1000 -log_step 50
# ./eval_by_mc -dataset ../data/nethept -epsilon $EPS -delta 0.001 -k_edges 50 -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -edges_mode MCGreedy_2000 -log_step 50
# ./eval_by_mc -dataset ../data/nethept -epsilon $EPS -delta 0.001 -k_edges 50 -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -edges_mode MCGreedy_3000 -log_step 50
# ./eval_by_mc -dataset ../data/nethept -epsilon $EPS -delta 0.001 -k_edges 50 -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -edges_mode MCGreedy_4000 -log_step 50
# ./eval_by_mc -dataset ../data/nethept -epsilon $EPS -delta 0.001 -k_edges 50 -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -edges_mode MCGreedy_5000 -log_step 50
# RAND_SEED=2021
