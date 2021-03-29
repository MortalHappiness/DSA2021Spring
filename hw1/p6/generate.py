import random

n = 1000
q = 1000

print(n, q)
L = random.choices(range(-10**5, 10**5+1), k=n)
print(" ".join([str(ch) for ch in L]))
N = len(L)
for _ in range(q):
    op = random.choice(["Insert", "Delete", "Reverse", "Query"])
    if op == "Insert":
        i = random.randint(1, N + 1)
        x = random.randint(-10**5, 10**5)
        N += 1
        print(f"Insert {i} {x}")
    elif op == "Delete":
        i = random.randint(1, N)
        N -= 1
        print(f"Delete {i}")
    elif op == "Reverse":
        l = random.randint(1, N)
        r = random.randint(1, N)
        if l > r:
            l, r = r, l
        print(f"Reverse {l} {r}")
    else:
        l = random.randint(1, N)
        r = random.randint(1, N)
        if l > r:
            l, r = r, l
        k = random.randint(1, r - l + 1)
        print(f"Query {l} {r} {k}")
