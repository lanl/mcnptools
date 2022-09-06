from mcnptools import Mctal, MctalTally

# construct the mctal class from mctal file "python_example_mctal_1.mcnp.mctal"
m = Mctal("example_mctal_1.mcnp.mctal")

tfc = MctalTally.TFC
# alias for -1

# get tally 4 from the mctal file
t4 = m.GetTally(4)

# get the energy bins of tally 4
t4_e = t4.GetEBins()

# loop over energy bin indices to store and print tally bin value
# using the TFC bin for all other bins

# store the tally values with list comprehension
#                        f    d    u    s    m    c   e   t
t4_evals = [t4.GetValue(tfc, tfc, tfc, tfc, tfc, tfc, i, tfc) for i in range(len(t4_e))]

# print the tally values
for i in range(len(t4_evals)):
    print(t4_evals[i])
