cd ../build
EPS=0.5
K_SEED=7
K_EDGES=14
BETA=200
NUM_CAND_EDGES=0
RAND_SEED=2022
SEED_MODE="RAND"

./eval_by_mc -dataset ../data/wiki_vote -epsilon 0.5 -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -edges_mode "MCGreedy_500" -log_step $K_EDGES -eval_mode MC -beta $BETA