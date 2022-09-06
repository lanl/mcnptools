from mcnptools import Meshtal, MeshtalTally
from sys import stdout

# construct the meshtal class from meshtal file "python_example_meshtal.mcnp.meshtal"
m = Meshtal("example_meshtal.mcnp.meshtal")

# get tally 4 from the meshtal file
t4 = m.GetTally(4)

# get the x and y bin centers
x = t4.GetXRBins()
y = t4.GetYZBins()

# loop over x and y bins indices and print the tally value for
# z index of 5
for i in range(len(x)):
    for j in range(len(y)):
        stdout.write("{:12.5e}".format(t4.GetValue(i, j, 5)))
    stdout.write("\n")
