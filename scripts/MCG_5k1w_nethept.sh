cd ../build

EPS=0.5
NUM_CAND_EDGES=10000
K_EDGES=50
RAND_SEED=2023
SEED_MODE="IM"
SEED_MODE="OUTDEG"

for NUM_MC in $(seq 6000 1000 10000)
do
    for RAND_SEED in 2021 2022 2023 2024 2025
    do
    ./MCGreedy_only -dataset ../data/nethept -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -num_mc $NUM_MC &
    done
    wait
done