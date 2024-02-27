cd ../build

K_EDGES=50
K_SEED=50
NUM_CAND_EDGES=0
NUM_SAMPLES=0

# SEED_MODE="RAND"
# Comparison of three RIS-based methods
SEED_MODE="RAND"
RAND_SEED=2021
EPS=0.1
K_EDGES=50
NUM_SAMPLES=102400


for K_EDGES in 500 1000 1500 2000;
do
    for RAND_SEED in $(seq 2021 2025);
    do
        # ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AIS_plus -num_samples $NUM_SAMPLES
        # ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AIS -num_samples $NUM_SAMPLES
        ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AISPAC -num_samples $NUM_SAMPLES
        ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AugIM -num_samples $NUM_SAMPLES
    done &
done
wait

for K_EDGES in 500 1000 1500 2000;
do
    for RAND_SEED in $(seq 2026 2030);
    do
        # ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AIS_plus -num_samples $NUM_SAMPLES
        # ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AIS -num_samples $NUM_SAMPLES
        ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AISPAC -num_samples $NUM_SAMPLES
        ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AugIM -num_samples $NUM_SAMPLES
    done &
done
wait

for K_EDGES in 500 1000 1500 2000;
do
    for RAND_SEED in $(seq 2021 2025);
    do
        # ./eval_by_mc -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -num_samples $NUM_SAMPLES -edges_mode "AIS_plus" -log_step $K_EDGES -eval_mode MC &
        # ./eval_by_mc -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -num_samples $NUM_SAMPLES -edges_mode "AIS" -log_step $K_EDGES -eval_mode MC &
        ./eval_by_mc -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -num_samples $NUM_SAMPLES -edges_mode "AISPAC" -log_step $K_EDGES -eval_mode MC &
        ./eval_by_mc -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -num_samples $NUM_SAMPLES -edges_mode "AugIM" -log_step $K_EDGES -eval_mode MC &
    done
    wait
done


for K_EDGES in 500 1000 1500 2000;
do
    for RAND_SEED in $(seq 2026 2030);
    do
        # ./eval_by_mc -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -num_samples $NUM_SAMPLES -edges_mode "AIS_plus" -log_step $K_EDGES -eval_mode MC &
        # ./eval_by_mc -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -num_samples $NUM_SAMPLES -edges_mode "AIS" -log_step $K_EDGES -eval_mode MC &
        ./eval_by_mc -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -num_samples $NUM_SAMPLES -edges_mode "AISPAC" -log_step $K_EDGES -eval_mode MC &
        ./eval_by_mc -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -num_samples $NUM_SAMPLES -edges_mode "AugIM" -log_step $K_EDGES -eval_mode MC &
    done
    wait
done
