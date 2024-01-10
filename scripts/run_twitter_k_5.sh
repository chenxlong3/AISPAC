cd ../build

EPS=0.5
NUM_CAND_EDGES=0
BETA=1
K_EDGES=5
# SEED_MODE="IM"
SEED_MODE="OUTDEG"

for RAND_SEED in 2021 2022 2023 2024 2025;
do
    ./IMA_only -dataset ../data/twitter -epsilon $EPS -delta 0.001 -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE
done