from matplotlib import pyplot as plt
import numpy as np
import os
import subprocess
import struct

# Estou acostumado a usar classes em Python


class Teste:
   def __init__(self):
        c_program_executable = os.path.join("./Trabalho_Pratico_II")
        os.system("make")
        result = subprocess.run(
            ["./Trabalho_Pratico_II"],
            capture_output=True,
            text=True,
            check=True,
        )
        print(result.stdout)

        arquivos: list[str] = os.listdir("./executions/")
        formato: str = '<10sqq' # 10 char, e dois long long
        tamanho = struct.calcsize(formato)
        dados: list = []


        print(f"Rodando teste para os arquivos: {arquivos}")
        for arquivo in arquivos:
            caminho: str = os.path.join("./executions", arquivo)
            with open(caminho, "rb") as f:
                while True:
                    chunk = f.read(tamanho)
                    if not chunk:
                        # Fim do arquivo
                        break

                    # Desempacota os dados
                    dados_tupla = struct.unpack(formato, chunk)
                    # Limpa a string
                    nome_limpo = dados_tupla[0].decode('utf-8').strip('\x00')
                    # Guarda os dados de forma organizada
                    resultado = {
                        "algoritmo": nome_limpo,
                        "trocas": dados_tupla[1],
                        "comparacoes": dados_tupla[2]
                    }
                    dados.append(resultado)
                    print(dados)





if __name__ == "__main__":
    test_instance: Teste = Teste()
