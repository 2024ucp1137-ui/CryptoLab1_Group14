import random, subprocess

n = int(open("outputs/prime_candidate.txt").read().strip())
BASES = [2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71]

def mr(n, a):
    d, s = n - 1, 0
    while d % 2 == 0:
        d //= 2; s += 1
    x = pow(a, d, n)
    if x in (1, n - 1): return True
    for _ in range(s - 1):
        x = pow(x, 2, n)
        if x == n - 1: return True
    return False

print("Bit length:", n.bit_length(), "[OK]" if n.bit_length() == 512 else "[FAIL]")
print("Odd:", n % 2 == 1)

for a in BASES:
    print(f"MR base {a}: {'PASS' if mr(n, a) else 'FAIL'}")

extra = [random.randrange(2, n - 1) for _ in range(40)]
print("40 extra random bases:", "PASS" if all(mr(n, a) for a in extra) else "FAIL")

# Independent check with OpenSSL's command-line tool (if available)
try:
    out = subprocess.run(["openssl", "prime", str(n)], capture_output=True, text=True)
    print("openssl prime:", out.stdout.strip()[-20:])
except FileNotFoundError:
    print("openssl not found, skip")
