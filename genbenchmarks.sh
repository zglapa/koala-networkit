#!/bin/bash

declare -A DENSITIES_VALUES
DENSITIES=(dens01 dens03 dens05 dens07 dens09)
DENSITIES_VALUES=(["dens01"]="0.1" ["dens03"]="0.3" ["dens05"]="0.5" ["dens07"]="0.7" ["dens09"]="0.9")
SIZES=(10 20 30 40 50 60)
VERSIONS=(v1 v2 v3 v4 v5)

generate() {
    for density in "${DENSITIES[@]}"; do
        rm -r "input/generated/${density}"
        mkdir "input/generated/${density}"
        for size in "${SIZES[@]}"; do
            mkdir "input/generated/${density}/n${size}"
        done
    done
    
    for density in "${DENSITIES[@]}"; do
        probability=${DENSITIES_VALUES[$density]}
        for size in "${SIZES[@]}"; do
            for version in "${VERSIONS[@]}"; do
                filename="input/generated/${density}/n${size}/n_${size}_${version}.col"
                touch "${filename}"
                echo "./build/benchmark/random_graph_generator ${size} ${probability} ${filename}"
                ./build/benchmark/random_graph_generator ${size} ${probability} "${filename}"
            done
        done
    done
}
run() {
    for density in "${DENSITIES[@]}"; do
        density_value=${DENSITIES_VALUES[$density]}
        echo "Density $density_value"
        rm -r "output/${density}"
        mkdir "output/${density}"
        for size in "${SIZES[@]}"; do
            echo "n_${size}"; python3 runner.py "input/generated/${density}/n${size}" > "output/${density}/n${size}.out"
        done
    done
}


# Check if an argument was provided
if [ $# -eq 0 ]; then
    echo "Correct usage is $0 [--generate | --run]"
    exit 1
fi

# Process the arguments
case "$1" in
    "--generate")
        generate;
    ;;
    "--run")
        run;
    ;;
    *)
        echo "Invalid argument. Available options: --generate, --run"
    ;;
esac
