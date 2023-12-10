import matplotlib.pyplot as plt

x1 = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
y1 = [98, 106, 104, 109, 95, 106, 111, 108, 108, 105, 100, 99, 106, 108, 106]

x2 = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
y2 = [104, 106, 108, 107, 105, 102, 102, 108, 110, 98, 107, 100, 101, 107, 101]

x3 = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
y3 = [98, 109, 104, 100, 104, 108, 105, 100, 100, 102, 112, 105, 103, 106, 104]

plt.scatter(x1, y1, label="Beh 1", color="red", marker="x")
plt.scatter(x2, y2, label="Beh 2", color="green", marker="x")
plt.scatter(x3, y3, label="Beh 3", color="blue", marker="x")

plt.xlabel('Běh simulace [hodiny]')
plt.ylabel('Počet zaparkovaných aut')
plt.title('Experiment 3 - Parkoviště 10 x 17')

plt.ylim(bottom=70)

plt.legend()

plt.show()

total_avg = (sum(y1) + sum(y2) + sum(y3)) / (len(y1) + len(y2) + len(y3))
print(total_avg)
