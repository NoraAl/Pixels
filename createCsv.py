import sys
import os.path

if __name__ == "__main__":

    if len(sys.argv) != 2:
        print "usage: create_csv <base_path>"
        sys.exit(1)

    BASE_PATH=sys.argv[1]
    # SEPARATOR=","

    # label = 0

for filename in os.listdir(BASE_PATH):
    abs_path = "%s/%s" % (BASE_PATH, filename)
    print "%s" % (abs_path)
            # label = label + 1