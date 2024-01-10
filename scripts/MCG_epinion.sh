cd ../build

EPS=0.5
NUM_CAND_EDGES=20000
K_SEED=76
K_EDGES=152
RAND_SEED=2021
SEED_MODE="IM"
SEED_MODE="RAND"

NUM_MC=500
# ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta 6000
for NUM_MC in 80000 100000
do
./MCGreedy_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta 6000 -num_mc $NUM_MC &
done