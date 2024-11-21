import numpy as np
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA
from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_score


# Правило 1: Ограничение по координатам
def generate_cloud_rule1(num_points):
    x1_range = (0, 4)
    x2_range = (8, 12)
    x3_range = (3, 7)
    x4_range = (-2, 2)
    x5_range = (2, 4)

    points = np.random.uniform(
        [x1_range[0], x2_range[0], x3_range[0], x4_range[0], x5_range[0]],
        [x1_range[1], x2_range[1], x3_range[1], x4_range[1], x5_range[1]],
        size=(num_points, 5)
    )
    return points

# Правило 2: Зависимая переменная
def generate_cloud_rule2(num_points):
    x1_range = (5, 9)
    x2_range = (8, 12)
    x3_range = (13, 17)
    x5_range = (3, 5)

    points = []
    for _ in range(num_points):
        x1 = np.random.uniform(*x1_range)
        x2 = np.random.uniform(*x2_range)
        x3 = np.random.uniform(*x3_range)
        x5 = np.random.uniform(*x5_range)
        x4 = 0.5 * x1 + 0.2 * x3  # Зависимость x4 от x1 и x3
        points.append([x1, x2, x3, x4, x5])

    return np.array(points)

# Правило 3: Ограничение по расстоянию
def generate_cloud_rule3(num_points):
    x1_range = (3, 7)
    x2_range = (8, 12)
    x3_range = (8, 12)
    x5_range = (1, 3)
    x4_range = (6, 10)

    points = []
    while len(points) < num_points:
        x1 = np.random.uniform(*x1_range)
        x2 = np.random.uniform(*x2_range)
        x3 = np.random.uniform(*x3_range)
        x5 = np.random.uniform(*x5_range)
        x4 = np.random.uniform(*x4_range)

        # Процедура на расстановку от центра
        distance = np.sqrt(x1 ** 2 + x2 ** 2 + x3 ** 2 + x4 ** 2 + x5 ** 2)
        if distance <= 20:
            points.append([x1, x2, x3, x4, x5])

    return np.array(points)

def apply_pca(points, n_components=2):
    pca = PCA(n_components=n_components)
    return pca.fit_transform(points)

def graph_clusters(clouds_2d, labels):
    plt.figure(figsize=(10, 6))
    unique_labels = np.unique(labels)
    colors = ['r', 'g', 'b', 'c', 'm', 'y', 'k']

    for i, label in enumerate(unique_labels):
        cluster_points = clouds_2d[labels == label]
        plt.scatter(
            cluster_points[:, 0],
            cluster_points[:, 1],
            color=colors[i % len(colors)],
            label=f'Кластер {label + 1}',
            alpha=0.6,
            edgecolor='w',
            s=100
        )

    plt.title("Визуализация кластеров с помощью KMeans (K=3)")
    plt.xlabel('Главная компонента 1')
    plt.ylabel('Главная компонента 2')
    plt.legend()
    plt.grid(True)
    plt.show()

def elbow_method(clouds_2d):
    distortions = []
    silhouette_scores = []
    max_k = 10

    for k in range(2, max_k + 1):
        kmeans = KMeans(n_clusters=k, random_state=42)
        kmeans.fit(clouds_2d)

        distortions.append(kmeans.inertia_)
        silhouette_avg = silhouette_score(clouds_2d, kmeans.labels_)
        silhouette_scores.append(silhouette_avg)

    # Процесс анализа
    plt.figure(figsize=(14, 6))

    # Метод локтя
    plt.subplot(1, 2, 1)
    plt.plot(range(2, max_k + 1), distortions, marker='o')
    plt.title('Метод локтя')
    plt.xlabel('Количество кластеров (K)')
    plt.ylabel('Сумма квадратов расстояний до центров кластеров')
    plt.xticks(range(2, max_k + 1))
    plt.grid(True)

    # Коэффициент силуэта
    plt.subplot(1, 2, 2)
    plt.plot(range(2, max_k + 1), silhouette_scores, marker='o', color='orange')
    plt.title('Коэффициент силуэта')
    plt.xlabel('Количество кластеров (K)')
    plt.ylabel('Средний коэффициент силуэта')
    plt.xticks(range(2, max_k + 1))
    plt.grid(True)

    plt.tight_layout()
    plt.show()

def main():
    N = 3  # количество правил генерации
    num_points = 50

    # Генерация облаков точек по каждому правилу
    cloud_rule1 = generate_cloud_rule1(num_points)
    cloud_rule2 = generate_cloud_rule2(num_points)
    cloud_rule3 = generate_cloud_rule3(num_points)

    # Применение PCA для снижения размерности до 2D
    cloud_rule1_2d = apply_pca(cloud_rule1)
    cloud_rule2_2d = apply_pca(cloud_rule2)
    cloud_rule3_2d = apply_pca(cloud_rule3)

    # Объединение всех облаков в одно
    all_clouds_2d = np.vstack([cloud_rule1_2d, cloud_rule2_2d, cloud_rule3_2d])

    # Визуализация кластеров с использованием KMeans (K=3)
    kmeans = KMeans(n_clusters=3, random_state=52)
    kmeans_labels = kmeans.fit_predict(all_clouds_2d)
    graph_clusters(all_clouds_2d, kmeans_labels)

    # Анализ оптимального числа кластеров
    elbow_method(all_clouds_2d)

if __name__ == "__main__":
    main()
