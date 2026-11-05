import numpy as np
from scipy.stats import chisquare
from collections import Counter

files = ["urandom.bin", "openssl.bin", "cng.bin"]

for f in files:
    data = np.fromfile(f, dtype=np.uint8)
    print(f"\n===== {f}  ({len(data)} bytes) =====")

    # 1. Byte frequency + chi-square (expected 1/256 each)
    counts = np.bincount(data, minlength=256)
    chi, p = chisquare(counts)
    print(f"Byte frequency: min={counts.min()} max={counts.max()} expected={len(data)/256:.1f}")
    print(f"Chi-square = {chi:.2f}, p-value = {p:.4f}")

    # 2. Bit distribution
    bits = np.unpackbits(data)
    ones = bits.sum()
    print(f"Bits: ones={ones} zeros={len(bits)-ones} ratio={ones/len(bits):.5f}")

    # 3. Repeated patterns (count repeated 4-byte and 8-byte blocks)
    for k in (4, 8):
        grams = [data[i:i+k].tobytes() for i in range(0, len(data)-k, k)]
        c = Counter(grams)
        rep = sum(1 for v in c.values() if v > 1)
        print(f"Repeated non-overlapping {k}-byte blocks: {rep}")

    # 4. Serial correlation (lag 1) and Shannon entropy
    x = data.astype(float)
    corr = np.corrcoef(x[:-1], x[1:])[0, 1]
    print(f"Serial correlation (lag 1) = {corr:.6f}")
    prob = counts / counts.sum()
    entropy = -(prob[prob > 0] * np.log2(prob[prob > 0])).sum()
    print(f"Shannon entropy = {entropy:.6f} bits/byte (ideal 8)")

    # Optional: byte-frequency histogram
    # import matplotlib.pyplot as plt; plt.bar(range(256), counts); plt.savefig(f + ".png")
