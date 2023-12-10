import matplotlib.pyplot as plt

x1 = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
y1 = [96, 94, 83, 88, 83, 88, 96, 91, 90, 93, 81, 94, 92, 87, 90]

x2 = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
y2 = [93, 93, 85, 92, 98, 90, 91, 91, 84, 88, 87, 88, 88, 92, 83]

x3 = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
y3 = [97, 89, 88, 96, 93, 91, 93, 95, 91, 92, 86, 93, 80, 90, 99]

plt.scatter(x1, y1, label="Beh 1", color="red", marker="x")
plt.scatter(x2, y2, label="Beh 2", color="green", marker="x")
plt.scatter(x3, y3, label="Beh 3", color="blue", marker="x")

plt.xlabel('Běh simulace [hodiny]')
plt.ylabel('Počet zaparkovaných aut')
plt.title('Experiment 2 - Parkoviště 8 x 15')

plt.ylim(bottom=70)

plt.legend()

plt.show()

total_avg = (sum(y1) + sum(y2) + sum(y3)) / (len(y1) + len(y2) + len(y3))
print(total_avg)
