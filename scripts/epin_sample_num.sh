cd ../build

EPS=0.5
NUM_CAND_EDGES=0
K_EDGES=50
RAND_SEED=2024
K_SEED=50
SEED_MODE="RAND"
# SEED_MODE="OUTDEG"

# for RAND_SEED in $(seq 2021 2030);
# do
#     for NUM_SAMPLES in 100 200 400 800 1600 3200 6400 12800 25600 51200 102400
#     do
#     ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AIS_plus -num_samples $NUM_SAMPLES
#     # ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AISPAC -num_samples $NUM_SAMPLES
#     # ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AugIM -num_samples $NUM_SAMPLES
#     done &
# done
# wait
# Evaluation

for RAND_SEED in $(seq 2021 2030);
# RAND_SEED=2021
do
    for NUM_SAMPLES in 100 200 400 800 1600 3200 6400 12800 25600 51200 102400
    do
    ./eval_by_mc -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AIS -num_samples $NUM_SAMPLES -edges_mode "AIS_plus" -log_step $K_EDGES -eval_mode MC &
    done
    wait
done