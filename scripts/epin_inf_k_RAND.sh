cd ../build

K_SEED=50
NUM_CAND_EDGES=0
NUM_SAMPLES=0
EPS=0.1

# SEED_MODE="RAND"
# Comparison of three RIS-based methods
RAND_SEED=2021
EPS=0.1
K_EDGES=50

# Scalability
NUM_SAMPLES_AIS=102400
SEED_MODE="RAND"
EPS=0.1
for RAND_SEED in 2021 2022 2023 2024 2025;
do
    # for K_EDGES in 5 10 20 40 60 80 100;
    for K_EDGES in 5;
    do
        ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method OPIM -num_samples $NUM_SAMPLES
        # ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AIS -num_samples $NUM_SAMPLES_AIS
        # ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AIS_plus -num_samples $NUM_SAMPLES_AIS
    done &
done
wait

for RAND_SEED in 2026 2027 2028 2029 2030;
do
    # for K_EDGES in 5 10 20 40 60 80 100;
    for K_EDGES in 5;
    do
        ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method OPIM -num_samples $NUM_SAMPLES
        # ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AIS -num_samples $NUM_SAMPLES_AIS
        # ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AIS_plus -num_samples $NUM_SAMPLES_AIS
    done &
done
wait

# Evaluation
# for K_EDGES in 5 10 20 40 60 80 100;
for K_EDGES in 5;
do
    for RAND_SEED in $(seq 2021 2030);    
    do
        ./eval_by_mc -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AISPAC -num_samples $NUM_SAMPLES -edges_mode "AISPAC" -log_step $K_EDGES -eval_mode MC &
        ./eval_by_mc -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AISPAC -num_samples $NUM_SAMPLES -edges_mode "AugIM" -log_step $K_EDGES -eval_mode MC &
        # ./eval_by_mc -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AISPAC -num_samples $NUM_SAMPLES_AIS -edges_mode "AIS" -log_step $K_EDGES -eval_mode MC &
        # ./eval_by_mc -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AISPAC -num_samples $NUM_SAMPLES_AIS -edges_mode "AIS_plus" -log_step $K_EDGES -eval_mode MC &
    done
done