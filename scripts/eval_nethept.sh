cd ../build

# current_datetime=$(date +%Y-%m-%d-%H-%M)
# mkdir ../logs/$current_datetime
# touch ../logs/$current_datetime/logs.txt
SEED_MODE="RAND"
SEED_MODE="OUTDEG"
SEED_MODE="RAND"
RAND_SEED=2023
NUM_CAND_EDGES=10000
./eval_by_mc -dataset ../data/nethept -epsilon 0.5 -delta 0.001 -k_edges 50 -rand_seed $RAND_SEED -num_cand_edges 10000 -fast True -probability WC -seed_mode $SEED_MODE -edges_mode MCGreedy_1200 -log_step 5 -eval_mode MC
# for RAND_SEED in $(seq 2021 1 2025)
# do
# ./eval_by_mc -dataset ../data/nethept -epsilon 0.5 -delta 0.001 -k_edges 50 -rand_seed $RAND_SEED -num_cand_edges 10000 -fast True -probability WC -seed_mode $SEED_MODE -edges_mode MCGreedy_10000 -log_step 5 -eval_mode COV
# done

# for method in "MCGreedy_1000" "MCGreedy_2000" "MCGreedy_3000" "MCGreedy_4000" "MCGreedy_5000" "MCGreedy_6000" "MCGreedy_7000" "MCGreedy_8000" "MCGreedy_9000"
# do
#     for RAND_SEED in $(seq 2021 1 2025)
#     do
#     ./eval_by_mc -dataset ../data/nethept -epsilon 0.5 -delta 0.001 -k_edges 50 -rand_seed $RAND_SEED -num_cand_edges 10000 -fast True -probability WC -seed_mode $SEED_MODE -edges_mode $method -log_step 5 -eval_mode COV
#     done
# done
# ./eval_by_mc -dataset ../data/nethept -epsilon 0.5 -delta 0.001 -k_edges 50 -rand_seed 2023 -num_cand_edges 10000 -fast True -probability WC -seed_mode $SEED_MODE -edges_mode AIS-U -log_step 5 -eval_mode COV
# ./eval_by_mc -dataset ../data/nethept -epsilon 0.5 -delta 0.001 -k_edges 50 -rand_seed 2023 -num_cand_edges 10000 -fast True -probability WC -seed_mode $SEED_MODE -edges_mode SINF -log_step 5 -eval_mode COV
# ./eval_by_mc -dataset ../data/nethept -epsilon 0.5 -delta 0.001 -k_edges 50 -rand_seed 2023 -num_cand_edges 10000 -fast True -probability WC -seed_mode $SEED_MODE -edges_mode UINF -log_step 5 -eval_mode COV
# ./eval_by_mc -dataset ../data/nethept -epsilon 0.5 -delta 0.001 -k_edges 50 -rand_seed 2023 -num_cand_edges 10000 -fast True -probability WC -seed_mode $SEED_MODE -edges_mode RAND -log_step 5 -eval_mode COV
# ./eval_by_mc -dataset ../data/nethept -epsilon 0.5 -delta 0.001 -k_edges 50 -rand_seed 2023 -num_cand_edges 10000 -fast True -probability WC -seed_mode $SEED_MODE -edges_mode OUTDEG -log_step 5 -eval_mode COV
# ./eval_by_mc -dataset ../data/nethept -epsilon 0.5 -delta 0.001 -k_edges 50 -rand_seed 2023 -num_cand_edges 10000 -fast True -probability WC -seed_mode $SEED_MODE -edges_mode PROB -log_step 5 -eval_mode COV


# ./eval_by_mc -dataset ../data/nethept -epsilon 0.5 -delta 0.001 -k_edges 50 -rand_seed $RAND_SEED -num_cand_edges $NUM_CAND_EDGES -fast True -probability WC -seed_mode $SEED_MODE -edges_mode "RWGreedy" -log_step 50 -eval_mode MC
