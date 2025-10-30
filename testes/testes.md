#  Conjunto de Testes PBM — Projeto 2: Codificador de Imagens Binárias

Este conjunto foi criado para validar o comportamento **recursivo e compacto** do codificador de imagens binárias em PBM (`P1 ASCII`), garantindo que cada região seja processada até atingir uniformidade total.  
O formato de saída esperado utiliza os símbolos:

- **P** → região preta (`1`)  
- **B** → região branca (`0`)  
- **X** → região mista, subdividida em quadrantes.

---

## 📂 Arquivos e Resultados Esperados

### 1️⃣ `uniform_white_5x4.pbm`
**Dimensão:** 5×4  
**Descrição:** Imagem totalmente branca (`0`).  
**Codificação esperada:** `B`  
**Recursão:** única verificação de homogeneidade.  

---

### 2️⃣ `uniform_black_5x4.pbm`
**Dimensão:** 5×4  
**Descrição:** Imagem totalmente preta (`1`).  
**Codificação esperada:** `P`  
**Recursão:** única verificação de homogeneidade.  

---

### 3️⃣ `checker_8x8.pbm`
**Dimensão:** 8×8  
**Descrição:** Padrão xadrez alternando 1 e 0.  
**Codificação esperada:** `XPBPBBPBPPBPBBPBP`  
**Recursão:** total, até nível unitário.  

---

### 4️⃣ `vertical_half_left_black_right_white_7x5.pbm`
**Dimensão:** 7×5  
**Descrição:** Metade esquerda preta (`1`), metade direita branca (`0`).  
**Codificação esperada:** `XPB`  
**Recursão:** nível 1, divisão vertical.  

---

### 5️⃣ `horizontal_half_top_white_bottom_black_5x7.pbm`
**Dimensão:** 5×7  
**Descrição:** Metade superior branca (`0`), inferior preta (`1`).  
**Codificação esperada:** `XBP`  
**Recursão:** nível 1, divisão horizontal.  

---

### 6️⃣ `letter_J_6x10.pbm`
**Dimensão:** 6×10  
**Descrição:** Letra “J” preta sobre fundo branco.  
**Codificação esperada:** `XBBPXPBPBPP`  
**Recursão:** múltiplos níveis, regiões brancas predominantes com detalhes pretos.  

---

### 7️⃣ `border_frame_9x9.pbm`
**Dimensão:** 9×9  
**Descrição:** Moldura preta e centro branco.  
**Codificação esperada:** `XPBPB`  
**Recursão:** dois níveis, mistura de bordas e interior.  

---

### 8️⃣ `diagonal_main_10x10.pbm`
**Dimensão:** 10×10  
**Descrição:** Pixels pretos na diagonal principal.  
**Codificação esperada:** `XBPBPBPPBPB`  
**Recursão:** profunda, devido à diagonal fina.  

---

### 9️⃣ `sparse_dots_13x9.pbm`
**Dimensão:** 13×9  
**Descrição:** Pontos pretos esparsos sobre fundo branco.  
**Codificação esperada:** `XBPBPBPBBP`  
**Recursão:** extensa, regiões predominantemente brancas.  

---

### 🔟 `quadrant_mosaic_11x11.pbm`
**Dimensão:** 11×11  
**Descrição:** Q1=preto, Q2=branco, Q3=xadrez, Q4=listras verticais.  
**Codificação esperada:** `XPBXXXPBPPB`  
**Recursão:** total, quadrantes aninhados com padrões distintos.  

---

## 🧠 Observações Gerais

- Todos os casos requerem **recursão**, mesmo os homogêneos.  
- As saídas estão no formato **compacto**, sem espaços, conforme especificação do codificador.  
- O símbolo `X` sempre indica subdivisão recursiva e concatenação direta das sub-regiões.  
- Dimensões ímpares testam cálculos de corte.  
- Comentários e espaçamentos dentro dos PBMs testam robustez do parser.

