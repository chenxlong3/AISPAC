cd ../build

EPS=0.5
NUM_CAND_EDGES=0
K_EDGES=50
BETA=1
SEED_MODE="IM"

for RAND_SEED in 2021 2022 2023 2024 2025;
do
    for EPS in 0.1 0.2 0.3 0.4;
    do
    ./IMA_only -dataset ../data/twitter -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE
    done
done