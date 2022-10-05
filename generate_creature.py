
import sys
import json
import random #.randrange(a,b), .uniform(a,b)
import struct

# genome = {b_neurons:[0.1,0.2,0.3], b_actions:[0.1,...,0.10], connections:[(0,1,0.0)(-3,12,1.0)]}

def save(G, filename):
    with open(filename, "wb") as outfile:
        nb = bytearray(struct.pack('i', int(len(G["connections"]))))
        outfile.write(nb)
        for B in G["b_neurons"]:
            Bb = bytearray(struct.pack('f', B))
            outfile.write(Bb)
        for b in G["b_actions"]:
            bb = bytearray(struct.pack('f', b))
            outfile.write(bb)
        for C in G["connections"]:
            C0b = bytearray(struct.pack('f', float(C[0])))
            C1b = bytearray(struct.pack('f', float(C[1])))
            C2b = bytearray(struct.pack('f', C[2]))
            outfile.write(C0b)
            outfile.write(C1b)
            outfile.write(C2b)

def load(filename):
    G = {}
    G["b_neurons"] = []
    G["b_actions"] = []
    G["connections"] = []
    with open(filename, "rb") as infile:
        str_fmt = 'i3f10f'
        str_len = struct.calcsize(str_fmt)
        str_unpack = struct.Struct(str_fmt).unpack_from
        tmp = str_unpack(infile.read(str_len))

        n_conns = tmp[0]
        G["b_neurons"] = tmp[1:4]
        G["b_actions"] = tmp[4:14]
        tmp2 = struct.Struct(str(n_conns*3)+'f').unpackfrom(infile.read(struct.calcsize(str(n_conns*3)+'f')))
        for i in range(n_conns):
            C0 = tmp2[3*i]
            C1 = tmp2[3*i+1]
            C2 = tmp2[3*i+2]
            G["connections"].append((int(C0),int(C1),C2))
    return G

def generate_new(n_conns):
    G = {}
    G["b_neurons"] = []
    for i in range(3):
        G["b_neurons"].append(random.uniform(-4.0,4.0))
    G["b_actions"] = []
    for i in range(10):
        G["b_actions"].append(random.uniform(-4.0,4.0))
    G["connections"] = []
    for i in range(int(n_conns)):
        C = (random.randrange(-3,18), random.randrange(-3,10), random.uniform(-4.0,4.0))
        G["connections"].append(C)
    return G

def mitosis(parent_name, n_conns):
    mutation_rate = 0.05 #currently static
    G = load(parent_name)
    for i in range(3):
        if random.uniform(0.0,1.0) < mutation_rate:
            G["b_neurons"][i] += random.uniform(-0.5, 0.5)
            G["b_neurons"][i] = min(max(-4.0, G["b_neurons"][i]), 4.0)
    for i in range(10):
        if random.uniform(0.0,1.0) < mutation_rate:
            G["b_actions"][i] += random.uniform(-0.5, 0.5)
            G["b_actions"][i] = min(max(-4.0, G["b_actions"][i]), 4.0)
    for i in range(len(G["connections"])):
        if random.uniform(0.0,1.0) < mutation_rate:
            C = list(G["connections"][i])
            r = random.randrange(0,3)
            if r != 3:
                C[r] = random.randrange(-3, 10)
            else:
                C[r] += random.uniform(-0.5,0.5)
                C[r] = min(max(-4.0, C[r]), 4.0)
            G["connections"][i] = (C[0],C[1],C[2])
    if random.uniform(0.0,1.0) < mutation_rate:
        if random.randrange(0,2) == 1:
            # add a connection
            C0 = random.randrange(-3, 18)
            C1 = random.randrange(-3, 10)
            C2 = random.uniform(-4.0,4.0)
            G["connections"].append((C0,C1,C2))
        else:
            # delete one at random
            del G["connections"][random.randrange(len(G["Connections"]))]
    return G

def mate(parent_name1, parent_name2, n_conns):
    return None

def main():
    if len(sys.argv) < 3 or len(sys.argv) > 5:
        print("Usage: python generate_creature.py <num_connections> [parent_1.gene] [parent_2.gene] <child.gene>")
        exit(0)

    # generate new creature
    elif len(sys.argv) == 3:
        save(generate_new(sys.argv[1]), sys.argv[-1])

    # parthenogenetic reproduction with mutation
    elif len(sys.argv) == 4:
        save(mitosis(sys.argv[2], sys.argv[1]), sys.argv[-1])

    # sesxual reproduction with mutation
    elif len(sys.argv) == 5:
        save(mate(sys.argv[2], sys.argv[3], sys.argv[1]), sys.argv[-1])

    else:
        print("This code should never run\n")

if __name__ == "__main__":
    main()
