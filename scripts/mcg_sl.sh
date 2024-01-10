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
# ./IMA_only -dataset ../data/slashdot -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
# for NUM_MC in 1000 5000
for NUM_MC in 50000
do
./MCGreedy_only -dataset ../data/slashdot -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA -num_mc $NUM_MC &
done
wait
./eval_by_mc -dataset ../data/slashdot -epsilon 0.5 -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -edges_mode "MCGreedy_50000" -log_step $K_EDGES -eval_mode MC -beta $BETA
# ./eval_by_mc -dataset ../data/slashdot -epsilon 0.5 -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -edges_mode "MCGreedy_5000" -log_step $K_EDGES -eval_mode MC -beta $BETA
# ./eval_by_mc -dataset ../data/slashdot -epsilon 0.5 -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -edges_mode "MCGreedy_1000" -log_step $K_EDGES -eval_mode MC -beta $BETA
# for RAND_SEED in 2021 2022 2023 2024 2025
# do
# ./IMA_only -dataset ../data/slashdot -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA
#     for NUM_MC in 500 1000
#     # for NUM_MC in 30000 50000
#     do
#     ./MCGreedy_only -dataset ../data/slashdot -epsilon $EPS -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -beta $BETA -num_mc $NUM_MC &
#     done
# wait
# done

# for RAND_SEED in 2021 2022 2023 2024 2025
# do
# ./eval_by_mc -dataset ../data/slashdot -epsilon 0.5 -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -edges_mode "MCGreedy_500" -log_step $K_EDGES -eval_mode MC -beta $BETA
# ./eval_by_mc -dataset ../data/slashdot -epsilon 0.5 -delta 0.001 -k_seed $K_SEED -k_edges $K_EDGES -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -edges_mode "MCGreedy_1000" -log_step $K_EDGES -eval_mode MC -beta $BETA
# done