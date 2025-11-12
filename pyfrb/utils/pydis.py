import marshal, dis, sys

if len(sys.argv) < 2:
    print("Usage: python dis_pyc.py file.pyc")
    sys.exit(1)

path = sys.argv[1]

with open(path, "rb") as f:
    f.read(16)  # skip the 16-byte header (magic, flags, timestamp, etc.)
    code = marshal.load(f)

dis.dis(code)
