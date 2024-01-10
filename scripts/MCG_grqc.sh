cd ../build

EPS=0.5
NUM_CAND_EDGES=10000
K_EDGES=50
RAND_SEED=2023
SEED_MODE="IM"
SEED_MODE="OUTDEG"

for RAND_SEED in 2021 2022 2023 2024 2025
do
    for NUM_MC in $(seq 1000 1000 5000)
    do
    ./MCGreedy_only -dataset ../data/GRQC -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -num_mc $NUM_MC
    done
done

# ./MCGreedy_only -dataset ../data/GRQC -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode IM -num_mc 1000

# ./eval_by_mc -dataset ../data/GRQC -epsilon $EPS -delta 0.001 -k_edges 50 -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode IM -edges_mode MCGreedy_1000 -log_step 50
# ./eval_by_mc -dataset ../data/GRQC -epsilon $EPS -delta 0.001 -k_edges 50 -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode IM -edges_mode MCGreedy_2000 -log_step 50
# ./eval_by_mc -dataset ../data/GRQC -epsilon $EPS -delta 0.001 -k_edges 50 -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode IM -edges_mode MCGreedy_3000 -log_step 50
# ./eval_by_mc -dataset ../data/GRQC -epsilon $EPS -delta 0.001 -k_edges 50 -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode IM -edges_mode MCGreedy_4000 -log_step 50
# ./eval_by_mc -dataset ../data/GRQC -epsilon $EPS -delta 0.001 -k_edges 50 -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode IM -edges_mode MCGreedy_5000 -log_step 50