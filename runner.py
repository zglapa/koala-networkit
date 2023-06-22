import os
import sys
import subprocess
import time

algorithms = {
    "Brown": "0",
    "Christofides": "1",
    "Brelaz": "2",
    "Korman": "3",
}
programPath = "./build/benchmark/benchmark_exact_vertex_coloring"

def execute_program_with_input_and_time(program_path, input_file_path, algorithm):
    start_time = time.time()
    
    with open(input_file_path, 'r') as input_file:
        try:
            subprocess.run([program_path, algorithms[algorithm], input_file_path], text=True, check=True)
        except subprocess.CalledProcessError as e:
            print(f"Error executing the program: {e}")
    
    end_time = time.time()
    execution_time = end_time - start_time
    return execution_time

def iterate_directory(directory):
    sums = [0,0,0,0]
    iterations = 0
    for root, dirs, files in os.walk(directory):
        for file in files:
            iterations += 1
            file_path = os.path.join(root, file)
            sys.stderr.write("iteration: " + str(iterations) + "\n")
            sums[0] += execute_program_with_input_and_time(programPath, file_path, "Brown")
            sums[1] += execute_program_with_input_and_time(programPath, file_path, "Christofides")
            sums[2] += execute_program_with_input_and_time(programPath, file_path, "Brelaz")
            sums[3] += execute_program_with_input_and_time(programPath, file_path, "Korman")
    print(f"Brown: {round(sums[0] / iterations,5)} seconds")
    print(f"Christofides: {round(sums[1] / iterations,5)} seconds")
    print(f"Brelaz: {round(sums[2] / iterations,5)} seconds")
    print(f"Korman: {round(sums[3] / iterations,5)} seconds") 
    print(f"{round(sums[0] / iterations,5)} & {round(sums[1] / iterations,5)} & {round(sums[2] / iterations,5)} & {round(sums[3] / iterations,5)} \\")   


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Please provide a directory path as an argument.")
        sys.exit(1)

    directory_path = sys.argv[1]
    iterate_directory(directory_path)
 