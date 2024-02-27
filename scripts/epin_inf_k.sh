cd ../build

K_EDGES=50
K_SEED=50
NUM_CAND_EDGES=0
NUM_SAMPLES=0
EPS=0.1

# SEED_MODE="RAND"
# Comparison of three RIS-based methods
SEED_MODE="OUTDEG"
RAND_SEED=2021
EPS=0.1
K_EDGES=50
# ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method est_S -num_samples $NUM_SAMPLES
K_EDGES=2000
# NUM_SAMPLES=102400
# ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AIS_plus -num_samples $NUM_SAMPLES
# ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AIS -num_samples $NUM_SAMPLES
# ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method OPIM -num_samples $NUM_SAMPLES

# for RAND_SEED in 2021 2022 2023 2024 2025;
# do
#     for K_EDGES in 5 10 20 30 40 50;
#     do
#         ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AIS_plus -num_samples $NUM_SAMPLES
#         # ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method OPIM -num_samples $NUM_SAMPLES
#     done &
# done
# wait

# for RAND_SEED in 2026 2027 2028 2029 2030;
# do
#     for K_EDGES in 5 10 20 30 40 50;
#     do
#         ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AIS_plus -num_samples $NUM_SAMPLES
#         # ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method OPIM -num_samples $NUM_SAMPLES
#     done &
# done

# for RAND_SEED in $(seq 2031 2035);
# do
#     for K_EDGES in 5 10 20 30 40 50;
#     do
#         ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AIS_plus -num_samples $NUM_SAMPLES
#         ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AIS -num_samples $NUM_SAMPLES
#         ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method OPIM -num_samples $NUM_SAMPLES
#     done &
# done

for K_EDGES in 5 10 20 30 40 50;
do
    for RAND_SEED in $(seq 2031 2035);    
    do
        ./eval_by_mc -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AIS -num_samples $NUM_SAMPLES -edges_mode "AIS" -log_step $K_EDGES -eval_mode MC &
        ./eval_by_mc -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AISPAC -num_samples $NUM_SAMPLES -edges_mode "AIS_plus" -log_step $K_EDGES -eval_mode MC &
        ./eval_by_mc -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AISPAC -num_samples $NUM_SAMPLES -edges_mode "AISPAC" -log_step $K_EDGES -eval_mode MC &
        ./eval_by_mc -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method AISPAC -num_samples $NUM_SAMPLES -edges_mode "AugIM" -log_step $K_EDGES -eval_mode MC &
    done
done

# Scalability
# SEED_MODE="RAND"
# EPS=0.1
# for RAND_SEED in 2021 2022 2023 2024 2025;
# do
#     for K_EDGES in 5 10 20 30 40 50;
#     do
#         ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method RIS -num_samples $NUM_SAMPLES
#     done &
# done
# wait

# for RAND_SEED in 2026 2027 2028 2029 2030;
# do
#     for K_EDGES in 5 10 20 30 40 50;
#     do
#         ./IMA_only -dataset ../data/Epinions -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -method RIS -num_samples $NUM_SAMPLES
#     done &
# done