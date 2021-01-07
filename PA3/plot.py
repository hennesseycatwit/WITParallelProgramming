import matplotlib
matplotlib.use('Agg')

import matplotlib.pyplot as plt

x = [2, 3, 4, 6, 12]

y = [21.36311, 2.66578, 1.83644, 1.19663, 0.88244]

fig, ax = plt.subplots()
ax.plot(x, y)

ax.grid()

plt.title('Ranks v time')


fig.savefig("test.png")
plt.show()