# script.py
import matplotlib.pyplot as plt
import csv

# Lire le fichier de résultats
results = []
with open('results.txt', 'r') as file:
    reader = csv.reader(file)
    for row in reader:
        results.append({'Algorithm': row[0], 'AvgTurnaround': float(row[1]), 'AvgWaiting': float(row[2])})

# Afficher les résultats
for result in results:
    print(f"Algorithme: {result['Algorithm']}")
    print(f"Temps moyen de rotation: {result['AvgTurnaround']:.2f}")
    print(f"Temps moyen d'attente: {result['AvgWaiting']:.2f}")
    print("----------------------------")

# Comparaison graphique
algorithms = [result['Algorithm'] for result in results]
avg_turnaround_times = [result['AvgTurnaround'] for result in results]
avg_waiting_times = [result['AvgWaiting'] for result in results]

plt.bar(algorithms, avg_turnaround_times, label='Temps moyen de rotation')
plt.bar(algorithms, avg_waiting_times, label='Temps moyen d\'attente')

plt.xlabel('Algorithmes')
plt.ylabel('Temps moyen')
plt.title('Comparaison des algorithmes')
plt.legend()
plt.show()
