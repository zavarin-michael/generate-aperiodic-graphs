import itertools
import re

def generate_bit_combinations(n, k):
    """Generates all numbers from combinations of n bits with k ones"""
    for bits in itertools.combinations(range(n), k):
        bitlist = ['0'] * n
        for i in bits:
            bitlist[i] = '1'
        yield int(''.join(bitlist), 2)

def extract_matching_lines(dot_filename, target_numbers, target_color):
    """
    Extracts lines from a .dot file where:
      - label matches a number from target_numbers
      - fillcolor matches target_color (either "" or "green")
    """
    pattern = re.compile(
        r'v\d+\s+\[.*?fillcolor\s*=\s*(?:"(.*?)"|(\w+))\s*,.*?label\s*=\s*(\d+).*?];'
    )
    results = []

    with open(dot_filename, 'r', encoding='utf-8') as f:
        for line in f:
            match = pattern.search(line)
            if match:
                fillcolor = match.group(1) if match.group(1) is not None else match.group(2)
                label_number = int(match.group(3))

                if label_number in target_numbers and fillcolor == target_color:
                    results.append(line.strip())

    return results

# ==== User input ====
n = int(input(" Enter bit string length (n): "))
k = int(input("Enter number of ones (k): "))
dot_file = input("Enter path to .dot file: ").strip()
color = input("Enter fillcolor to match (e.g., 'green' or leave empty): ").strip()

# ==== Execution ====
targets = set(generate_bit_combinations(n, k))
matches = extract_matching_lines(dot_file, targets, color)

# ==== Output ====
print(f"\nFound {len(matches)} matching lines:")
for line in matches:
    print(line)
