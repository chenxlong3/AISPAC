EPS=0.5
K_SEED=51
K_EDGES=102
BETA=2000
NUM_CAND_EDGES=0
SEED_MODE="RAND"
RAND_SEED=2023
NUM_MC=500
cd ../build

RAND_SEED=2021
./eval_by_mc -dataset ../data/slashdot -epsilon 0.5 -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -edges_mode "MCGreedy_40000" -log_step $K_EDGES -eval_mode MC -beta $BETA