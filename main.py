from matplotlib import pyplot as plt
import os
import subprocess
import struct
from typing import List, Dict

class Teste:
    def __init__(self) -> None:
        c_program_executable: str = "./Trabalho_Pratico_II"
        os.system("make")

        result: subprocess.CompletedProcess = subprocess.run(
            [c_program_executable],
            capture_output=True,
            text=True,
            check=True,
        )
        print(result.stdout)

        arquivos: List[str] = sorted(os.listdir("./executions/"))
        formato: str = '<10sqqd'
        # string de 10 bytes, 2 long long, 1 double
        tamanho: int = struct.calcsize(formato)
        self.dados: Dict[str, List[Dict[str, object]]] = {}

        for arquivo in arquivos:
            caminho: str = os.path.join("./executions", arquivo)
            with open(caminho, "rb") as f:
                while True:
                    chunk: bytes = f.read(tamanho)
                    if not chunk:
                        break
                    dados_tupla: tuple = struct.unpack(formato, chunk)
                    nome_limpo: str = dados_tupla[0].decode('utf-8').strip('\x00')
                    resultado: Dict[str, object] = {
                        "trocas": dados_tupla[1],
                        "comparacoes": dados_tupla[2],
                        "tempo": dados_tupla[3]
                    }
                    if nome_limpo not in self.dados:
                        self.dados[nome_limpo] = []
                    self.dados[nome_limpo].append(resultado)
                    print(self.dados)


if __name__ == "__main__":

    test_instance: Teste = Teste()
    dados_por_algo: Dict[str, List[Dict[str, object]]] = test_instance.dados
    tamanhos: List[int] = [100, 1000, 5000, 10000, 50000, 100000]
    algos: list = sorted(dados_por_algo.keys())

    os.makedirs("./graficos", exist_ok=True)

    styles = {
        'bubble':    {'linestyle': '-',  'marker': 'o', 'color': 'red'},
        'insertion': {'linestyle': '--', 'marker': 'x', 'color': 'green'},
        'selection': {'linestyle': ':',  'marker': '^', 'color': 'blue'},
        'quicksort': {'linestyle': '-.', 'marker': 's', 'color': 'purple'},
        'heapsort':  {'linestyle': '-',  'marker': 'd', 'color': 'orange'}
    }

    # Gráfico completo com todos os algoritmos
    fig, axes = plt.subplots(3, 1, figsize=(16, 22), sharex=True)
    fig.suptitle('Análise de Performance de Algoritmos de Ordenação', fontsize=20)

    for algo in algos:
        style = styles.get(algo, {'linestyle': '-', 'marker': 'o'})

        swaps = [d["trocas"] for d in dados_por_algo[algo]]
        axes[0].plot(tamanhos, swaps, label=algo, linewidth=2, markersize=8, **style)

        comparacoes = [d["comparacoes"] for d in dados_por_algo[algo]]
        axes[1].plot(tamanhos, comparacoes, label=algo, linewidth=2, markersize=8, **style)

        tempos = [d["tempo"] for d in dados_por_algo[algo]]
        axes[2].plot(tamanhos, tempos, label=algo, linewidth=2, markersize=8, **style)

    axes[0].set_yscale('log')
    axes[0].set_title("Trocas por Algoritmo", fontsize=16)
    axes[0].set_ylabel("Número de Trocas (Escala Log)", fontsize=14)
    axes[0].legend(fontsize=12)
    axes[0].grid(True, which="both", linestyle='--', linewidth=0.5)

    axes[1].set_yscale('log')
    axes[1].set_title("Comparações por Algoritmo", fontsize=16)
    axes[1].set_ylabel("Número de Comparações (Escala Log)", fontsize=14)
    axes[1].legend(fontsize=12)
    axes[1].grid(True, which="both", linestyle='--', linewidth=0.5)

    axes[2].set_yscale('log')
    axes[2].set_title("Tempo de Execução por Algoritmo", fontsize=16)
    axes[2].set_ylabel("Tempo (s) (Escala Log)", fontsize=14)
    axes[2].set_xlabel("Tamanho do Vetor", fontsize=14)
    axes[2].legend(fontsize=12)
    axes[2].grid(True, which="both", linestyle='--', linewidth=0.5)

    plt.tight_layout(rect=[0, 0, 1, 0.96])
    plt.savefig("./graficos/relatorio_completo.png", dpi=300)
    plt.close()

    print("\nGráfico 'relatorio_completo.png' foi salvo com sucesso.")

    # Gráfico individual para Trocas (todos algoritmos)
    plt.figure(figsize=(12, 8))
    for algo in algos:
        style = styles.get(algo, {'linestyle': '-', 'marker': 'o'})
        swaps = [d["trocas"] for d in dados_por_algo[algo]]
        plt.plot(tamanhos, swaps, label=algo, linewidth=3, markersize=10, **style)

    plt.yscale('log')
    plt.title("Comparação de Trocas entre Algoritmos", fontsize=16)
    plt.ylabel("Número de Trocas (Escala Log)", fontsize=14)
    plt.xlabel("Tamanho do Vetor", fontsize=14)
    plt.legend(fontsize=12)
    plt.grid(True, which="both", linestyle='--', linewidth=0.5)
    plt.tight_layout()
    plt.savefig("./graficos/trocas_comparacao.png", dpi=300)
    plt.close()
    print("Gráfico 'trocas_comparacao.png' foi salvo com sucesso.")

    # Gráfico individual para Comparações (todos algoritmos)
    plt.figure(figsize=(12, 8))
    for algo in algos:
        style = styles.get(algo, {'linestyle': '-', 'marker': 'o'})
        comparacoes = [d["comparacoes"] for d in dados_por_algo[algo]]
        plt.plot(tamanhos, comparacoes, label=algo, linewidth=3, markersize=10, **style)

    plt.yscale('log')
    plt.title("Comparação de Comparações entre Algoritmos", fontsize=16)
    plt.ylabel("Número de Comparações (Escala Log)", fontsize=14)
    plt.xlabel("Tamanho do Vetor", fontsize=14)
    plt.legend(fontsize=12)
    plt.grid(True, which="both", linestyle='--', linewidth=0.5)
    plt.tight_layout()
    plt.savefig("./graficos/comparacoes_comparacao.png", dpi=300)
    plt.close()
    print("Gráfico 'comparacoes_comparacao.png' foi salvo com sucesso.")

    # Gráfico individual para Tempo (todos algoritmos)
    plt.figure(figsize=(12, 8))
    for algo in algos:
        style = styles.get(algo, {'linestyle': '-', 'marker': 'o'})
        tempos = [d["tempo"] for d in dados_por_algo[algo]]
        plt.plot(tamanhos, tempos, label=algo, linewidth=3, markersize=10, **style)

    plt.yscale('log')
    plt.title("Comparação de Tempo de Execução entre Algoritmos", fontsize=16)
    plt.ylabel("Tempo (s) (Escala Log)", fontsize=14)
    plt.xlabel("Tamanho do Vetor", fontsize=14)
    plt.legend(fontsize=12)
    plt.grid(True, which="both", linestyle='--', linewidth=0.5)
    plt.tight_layout()
    plt.savefig("./graficos/tempo_comparacao.png", dpi=300)
    plt.close()
    print("Gráfico 'tempo_comparacao.png' foi salvo com sucesso.")

    print(f"\nTodos os gráficos foram gerados: 1 completo + 3 individuais (trocas, comparações, tempo).")
