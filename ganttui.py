import os
import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QListWidgetItem
from gantt_ui import Ui_MainWindow
import pyqtgraph as pg

class GranttApp(QMainWindow, Ui_MainWindow):
    def __init__(self, algorithm_choices):
        super().__init__()
        self.setupUi(self)
        self.algorithm_choices = algorithm_choices
        self.populate_algorithm_list()

        self.selectButton.clicked.connect(self.on_select_button_clicked)

    def populate_algorithm_list(self):
        for algorithm in self.algorithm_choices:
            item = QListWidgetItem(algorithm["name"])
            self.algorithmList.addItem(item)
    def on_select_button_clicked(self):
         selected_item = self.algorithmList.currentItem()
         if selected_item is not None:
            selected_text = selected_item.data(0) # Le texte de l'élément sélectionné
            selected_index = [algo["name"] for algo in self.algorithm_choices].index(selected_text) # L'indice de l'élément sélectionné dans la liste self.algorithm_choices
            selected_algorithm = self.algorithm_choices[selected_index] # L'élément correspondant dans la liste self.algorithm_choices
            print(f"User selected: {selected_algorithm['name']}")
         

         print(f"Exécution de l'algorithme {selected_algorithm['name']} à partir du fichier {selected_algorithm['location']}")
         os.system(f"./algorithms/build/{selected_algorithm['name']} processes.txt")

         executable = f"./algorithms/build/{selected_algorithm['name']}"
         if os.path.exists(executable) and os.access(executable, os.X_OK):
              print (f"ok: ")
    # Exécuter le fichier
         else:
             print (f"no: ")
    # Afficher un message d'erreur



# Lire le fichier grantt.txt
         print(f"Fichier grantt.txt généré")
         f = open("./grantt.txt", "r")
         print(f"Chemin absolu du répertoire de travail : {os.path.abspath(os.getcwd())}")


# Lire le contenu du fichier
         content = f.read()
 
# Afficher le contenu du fichier
         print(content)

# Fermer le fichier
         f.close()

        # Lire le fichier grantt.txt et le convertir en une liste de dictionnaires
         print("Lecture du fichier grantt.txt")
         grantt_list = read_grantt_file("grantt.txt")
         print(f"Le fichier grantt.txt contient {len(grantt_list)} processus")
      
        # Tracer le diagramme de Gantt à partir de la liste de dictionnaires
         print("Tracé du diagramme de Gantt")
         print(f"User selected: {selected_algorithm['name']}")
         plot_grantt_chart(grantt_list, read_process_names("grantt.txt"),{selected_algorithm['name']})

         print(f"Diagramme de Gantt tracé et affiché")


def read_process_names(filename):
    # Cette fonction lit le fichier grantt.txt et extrait les noms des processus
    # Elle renvoie une liste de noms
    process_names = []
    with open(filename, "r") as f:
        lines = f.readlines()
        for line in lines:
            name = line.split()[0] # Le nom du processus est le premier mot de la ligne
            process_names.append(name)
    return process_names

def read_grantt_file(filename):
    # Cette fonction lit le fichier grantt.txt et le convertit en une liste de listes
    # Chaque liste contient les lettres E, A, P et T correspondant à un processus
    grantt_list = []
    with open(filename, "r") as f:
        lines = f.readlines()
        for line in lines:
            process_list = []
            for char in line:
                if char == "E" or char == "A" or char == "P" or char == "T":
                    process_list.append(char)
            grantt_list.append(process_list)
    return grantt_list

def plot_grantt_chart(grantt_list, process_names, algorithm_name):
    # Cette fonction trace le diagramme de Gantt à partir de la liste de listes et de la liste de noms
    # Elle utilise la bibliothèque PyQtGraph pour créer un graphique à barres horizontales avec les noms des processus sur l'axe des y et le temps sur l'axe des x
    # Elle utilise aussi des couleurs différentes pour représenter les états des processus : vert pour l'exécution, rouge pour l'attente
    # Elle affiche le graphique dans la fenêtre principale de l'application
    pg.setConfigOption('background', 'w')
    pg.setConfigOption('foreground', 'k')
    plot_widget = pg.PlotWidget()
  
    

    plot_widget.setLabel('left', 'Processus')
    plot_widget.setLabel('bottom', 'Temps')
    plot_widget.setXRange(0, len(grantt_list)) # Décaler l'axe des x d'une unité vers la droite
    plot_widget.setYRange(0, len(grantt_list) + 2) # Augmenter la hauteur de la fenêtre graphique
    plot_widget.showGrid(x=True, y=True)
   
    # Ajouter un titre avec le nom de l'algorithme sélectionné
    plot_widget.setTitle(f"Diagramme de Gantt pour l'algorithme {algorithm_name}")
     # Créer une liste de ticks pour l'axe des y avec les noms des processus
    y_ticks = []
    for i, name in enumerate(process_names):
        y_ticks.append((i + 1.5, name)) # Décaler les noms des processus vers le haut
    # Créer une liste de ticks pour l'axe des x avec les unités de temps
    x_ticks = []
    for i in range(len(grantt_list)):
        x_ticks.append((i, str(i + 1))) # Pas besoin de décaler les unités de temps
    # Appliquer les ticks aux axes
    plot_widget.getAxis('left').setTicks([y_ticks])
    plot_widget.getAxis('bottom').setTicks([x_ticks])
    for i, process in enumerate(grantt_list):
        y = i + 1.5 # Décaler les barres horizontales vers le haut
        for j, char in enumerate(process):
            x = j - 1 # Soustraire 1 à la coordonnée « j »
            width = 1 # La largeur des barres horizontales
            if char in ["E", "A"]: # Ne tracer que les états E et A
                if char == "E":
                   plot_widget.plot([x, x + width], [y, y], pen=pg.mkPen(color='g', width=15), align='left',name='Process Execute') # Vert pour les E, alignement à gauche
                elif char == "A":
                   plot_widget.plot([x, x + width], [y, y], pen=pg.mkPen(color='r', width=15), align='left',name='Process Wait') # Rouge pour les A, alignement à gauche
    legend = plot_widget.addLegend(offset=(0, 1))
    legend.addItem(pg.PlotDataItem(pen=pg.mkPen(color='g', width=15), name='Process Execute'), 'Process Execute')
    legend.addItem(pg.PlotDataItem(pen=pg.mkPen(color='r', width=15), name='Process Wait'), 'Process Wait')
    window.setCentralWidget(plot_widget)


if __name__ == "__main__":
    # Sample algorithm choices (replace with your actual algorithm choices)
    sample_algorithm_choices = [
        {"name": "FIFO", "location": "./algorithms/build/FIFO"},
        {"name": "SJF", "location": "./algorithms/build/SJF"}
        # Add more algorithms as needed
    ]

    app = QApplication(sys.argv)
    window = GranttApp(sample_algorithm_choices)
    window.show()
    sys.exit(app.exec_())
