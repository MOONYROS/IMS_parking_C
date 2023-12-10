import matplotlib.pyplot as plt

x1 = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
y1 = [108, 109, 102, 108, 107, 106, 108, 102, 101, 98, 112, 102, 107, 103, 105]

x2 = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
y2 = [105, 103, 98, 104, 107, 101, 98, 118, 104, 108, 107, 100, 101, 103, 101]

x3 = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
y3 = [106, 104, 106, 109, 107, 109, 106, 108, 104, 101, 103, 108, 106, 103, 102]

plt.scatter(x1, y1, label="Beh 1", color="red", marker="x")
plt.scatter(x2, y2, label="Beh 2", color="green", marker="x")
plt.scatter(x3, y3, label="Beh 3", color="blue", marker="x")

plt.xlabel('Běh simulace [hodiny]')
plt.ylabel('Počet zaparkovaných aut')
plt.title('Experiment 1 - Parkoviště 10 x 20')

plt.ylim(bottom=70)

plt.legend()

plt.show()

total_avg = (sum(y1) + sum(y2) + sum(y3)) / (len(y1) + len(y2) + len(y3))
print(total_avg)
