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

        arquivos: List[str] = os.listdir("./executions/")
        formato: str = '<10sqqd'  # 10 bytes string, 2 long long, 1 double
        tamanho: int = struct.calcsize(formato)
        self.dados: List[Dict[str, object]] = []

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
                        "algoritmo": nome_limpo,
                        "trocas": dados_tupla[1],
                        "comparacoes": dados_tupla[2],
                        "tempo": dados_tupla[3]
                    }
                    self.dados.append(resultado)


if __name__ == "__main__":
    # Cria a instância
    test_instance: Teste = Teste()
    dados: List[Dict[str, object]] = test_instance.dados
    tamanhos: List[int] = [100, 1000, 5000, 10000, 50000, 100000]
    algos: list = sorted(set(d["algoritmo"] for d in dados))

    os.makedirs("./graficos", exist_ok=True)

    # Gráfico de Trocas
    plt.figure(figsize=(12,6))
    for algo in algos:
        swaps: list = [d["trocas"] for d in dados if d["algoritmo"] == algo]
        plt.plot(tamanhos, swaps, marker='o', label=algo)
    plt.xlabel("Tamanho do vetor")
    plt.ylabel("Número de trocas")
    plt.title("Trocas por algoritmo")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig("./graficos/trocas.png")
    plt.close()

    # Gráfico de Tempo
    plt.figure(figsize=(12,6))
    for algo in algos:
        tempos: list = [d["tempo"] for d in dados if d["algoritmo"] == algo]
        plt.plot(tamanhos, tempos, marker='o', label=algo)
    plt.xlabel("Tamanho do vetor")
    plt.ylabel("Tempo de execução (s)")
    plt.title("Tempo por algoritmo")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig("./graficos/tempo.png")
    plt.close()

    # Gráfico de Comparações
    plt.figure(figsize=(12, 6))
    for algo in algos:
        comparacoes: list = [d["comparacoes"] for d in dados if d["algoritmo"] == algo]
        plt.plot(tamanhos, comparacoes, marker='o', label=algo)
    plt.xlabel("Tamanho do vetor")
    plt.ylabel("Número de comparações")
    plt.title("Comparações por algoritmo")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig("./graficos/comparacoes.png")
    plt.close()