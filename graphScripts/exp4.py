import matplotlib.pyplot as plt

x1 = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
y1 = [101, 113, 105, 108, 105, 109, 112, 112, 118, 99, 102, 111, 99, 107, 104]

x2 = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
y2 = [115, 106, 102, 111, 102, 103, 109, 105, 102, 100, 105, 106, 108, 99, 106]

x3 = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
y3 = [104, 113, 105, 100, 106, 107, 97, 103, 103, 104, 106, 104, 108, 105, 99]

plt.scatter(x1, y1, label="Beh 1", color="red", marker="x")
plt.scatter(x2, y2, label="Beh 2", color="green", marker="x")
plt.scatter(x3, y3, label="Beh 3", color="blue", marker="x")

plt.xlabel('Běh simulace [hodiny]')
plt.ylabel('Počet zaparkovaných aut')
plt.title('Experiment 4 - Parkoviště 10 x 18')

plt.ylim(bottom=70)

plt.legend()

plt.show()

total_avg = (sum(y1) + sum(y2) + sum(y3)) / (len(y1) + len(y2) + len(y3))
print(total_avg)
