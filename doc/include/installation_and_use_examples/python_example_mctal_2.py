from mcnptools import Mctal, MctalKcode

# construct the mctal class from the mctal file "python_example_mctal_2.mcnp.mctal"
m = Mctal("example_mctal_2.mcnp.mctal")

# get the kcode data
kc = m.GetKcode()

# alias for average combined keff
keff = MctalKcode.AVG_COMBINED_KEFF
# alias for average combined keff standard deviation
keff_std = MctalKcode.AVG_COMBINED_KEFF_STD

# loop over active cycles and print
for i in range(kc.GetSettle(), kc.GetCycles()):
    print(i, "  ", kc.GetValue(keff, i), "  ", kc.GetValue(keff_std, i))
