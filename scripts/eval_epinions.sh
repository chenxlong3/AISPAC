cd ../build
RAND_SEED=2021
NUM_CAND_EDGES=20000
# NUM_CAND_EDGES=0
K_EDGES=152
SEED_MODE="RAND"

./eval_by_mc -dataset ../data/Epinions -epsilon 0.5 -delta 0.001 -k_seed 76 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -edges_mode "MCGreedy_80000" -log_step 152 -eval_mode MC -beta 6000