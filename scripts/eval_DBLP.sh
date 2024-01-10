cd ../
rm -r ./build
mkdir ./build
cd ./build
cmake ../
make

current_datetime=$(date +%Y-%m-%d-%H-%M)
mkdir ../logs/$current_datetime
touch ../logs/$current_datetime/logs.txt
./eval_by_mc -dataset ../data/DBLP -epsilon 0.4 -delta 0.01 -k_edges 50 -rand_seed 2023 -edges_mode IMA | tee ../logs/$current_datetime/logs.txt

current_datetime=$(date +%Y-%m-%d-%H-%M)
mkdir ../logs/$current_datetime
touch ../logs/$current_datetime/logs.txt
./eval_by_mc -dataset ../data/DBLP -epsilon 0.4 -delta 0.01 -k_edges 50 -rand_seed 2023 -edges_mode RAND | tee ../logs/$current_datetime/logs.txt

current_datetime=$(date +%Y-%m-%d-%H-%M)
mkdir ../logs/$current_datetime
touch ../logs/$current_datetime/logs.txt
./eval_by_mc -dataset ../data/DBLP -epsilon 0.4 -delta 0.01 -k_edges 50 -rand_seed 2023 -edges_mode OUTDEG | tee ../logs/$current_datetime/logs.txt