#include <stdio.h>
#include <string.h>  // para usar a função strcmp() que compara duas strings.
#include <stdlib.h>  // para usar a função exit() que interrompe (aborta) a execução do programa.

typedef struct TpDADO {
            int L, C;
        } TpDADO;

typedef struct TpNODE {  /// estrutura da "caixa" do nodo
            TpDADO dado;
            struct TpNODE *prox; /// permite encadear caixas (*p para Link)
        } TpNODE;

typedef struct TpPILHA {   /// cabecalho (ou descritor) da estrutura de dados.
            TpNODE *topo;
          //  int len;    /// campos complementares do descritor.
          //  TpNODE *fim; /// Não precisam ser usados.
        } TpPILHA;

typedef unsigned char Tpixel; // tipo Tpixel para valores em [0, 255]

    typedef struct pgm {
      int w;     // largura da imagem em pixels (TC - Total de pixels em cada coluna da imagem).
      int h;     // altura da imagem em pixel   (TL - Total de pixels em cada linha da imagem).
      int max;   // valor de brilho máximo registrado na imagem (use 255 sempre).
      Tpixel* pData; // ponteiro para o reticulado (matriz) da imagem alocada como um vetor.
    } pgm;

void InicializaPilha(TpPILHA *p){
printf("\n ..... Pilha Inicializada.");
    p->topo = NULL;  /// topo da pilha está vazio.
   ///  p->len = 0;   /// outros campos do descritor.
   ///  p->fim = NULL;
}

int push(TpDADO x, TpPILHA *p) { /// empilhar x.

    TpNODE *paux;
    /// alocar memoria.
    paux = (TpNODE*) malloc(sizeof(TpNODE));
    if(!paux) {
        printf("\n ERRO na alocacao de um novo NODO na pilha.");
        return -1;
    }

    paux->dado = x;
    paux->prox = p->topo;
    p->topo = paux;

    return 1;/// sucesso
}

int pop(TpDADO *x, TpPILHA *p) { /// desempilhar x.

    if(!p->topo) {
            printf("\n ERRO no desempilhamento. Pilha vazia.");
            return -2;// nao se pode desempilhar em pilha vazia.
    }
    /// tem dados empilhados.
    TpNODE *paux;
    paux = p->topo;
    /// copiar o valor do dado armazenado no topo da pilha.
    *x = paux->dado;
    p->topo = paux->prox;
    paux->prox = NULL; /// desligar do encadeamento a caixa do topo.

    free(paux); /// desalocar da memória o NODO desempihado.
    return 2;
}


int WritePGM(const char* arq, const char* tpPGM, pgm* pPgm) {

printf("\n ..... (ARQUIVO GRAVADO)-->%s.", arq);
        FILE *f;

        f = fopen(arq, "w");  /// modo "w" --> abrir para escrita.
        if (!f) {
            printf("\n ERRO: Incapaz de abrir arquivo: %s.", arq);
            return -1;  /// Algum problema com o arquivo(ex: Caminho incorreto) ou o Arquivo nao existe.
        }

        /// Gravar no arquivo o cabeçalho de imagens PGM cinza.

        fprintf(f, "%s\n", tpPGM); ///  assinatura PGM:
                /// color "P3", cinza "P2" ou binario "P1"
        if (!strcmp(tpPGM, "P3")) {/// Se a imagem é colorida, largura está multiplicada por 3.
                                      
            fprintf(f,"%d %d\n", pPgm->w/3, pPgm->h); // É necessário dividir a largura por 3.

        }else {
            fprintf(f,"%d %d\n", pPgm->w, pPgm->h); // imagem binária ou cinza.
        }
        fprintf(f,"255\n"); // valor de brilho máximo na imagem (255).

        /// gravar no arquivo os valores dos pixels da imagem computada.
        for (unsigned int k = 0; k < (pPgm->w * pPgm->h); k++) {
            fprintf(f,"%d ", pPgm->pData[k]);
        }

        fclose(f);

        printf("\n ..... (ARQUIVO GRAVADO)-->%s.", arq);
        return(1);
}


int ReadPGMP2(const char* arq, pgm* pPgm) {
    // ReadPGMP2 lê uma imagem gravada em arquivo com formato PGM "P2"
    //      INPUT: arq é uma string que aponta o caminho e o nome do arquivo PGM P2 que vai ser lido.
    //      OUTPUT: fd retorna o descritor de arquivo (ponteiro) que atuará como tratador do arquivo arq.
    //              Se o arquivo arq não existir, ou não for encontrado no caminho fornecido,  ou não for um
    //              arquivo PGM "P2", a execução desta função é abortada e fd e pPgm ficam indefinidos.
    //              pPgm é um ponteiro para uma struct com campos definidos em pgm, a qual vai armazenar
    //              dados relevantes que estão gravados no arquivo PGM arq.

       int word;                    // inteiro de propósito geral.
       unsigned int nRead = 0;      // índice para o vetor/matriz de dados (pixels) da imagem.
       char readChars[256];         // buffer para ler caracteres do arquivo (propósito geral).

       FILE* fd;  /// descritor para um arquivo

       //abrir arquivo de entrada arq e checar se teve sucesso.
       fd = fopen(arq, "r");  // modo "r" --> abrir somente para leitura
       if (!fd) {
         printf("\n ERRO: Incapaz de abrir arquivo: %s.\n\n", arq);
         exit(1);  // arquivo inexistente ou não encontrado no caminho especificado.
         // encerra a execução do programa e fecha todos os arquivos abertos.
       }
       // se o arquivo existe, ele pode ser um PGM. Então,
      fscanf (fd, "%s", readChars); // leia a primeira linha (string) do arquivo fd cujo valor esperado é "P2"

      if ((strcmp(readChars, "P2"))) { // strcmp(s1,s2) compara os caracteres das strings s1 e s2.
                                       // retorna o valor zero se s1 é exatamente igual a s2.
        printf ("\n\nErro em PGM: %s. Formato nao suportado em:  ", readChars);
        fclose(fd);
        exit(2);  // código de erro para "Arquivo não é um PGM P2"
      }
       // o arquivo arq vinculado ao descritor fd tem assinatura PGM "P2", então
       fscanf (fd, "%s", readChars); // leia a segunda linha

       while (readChars[0] == '#') {   // se no cabeçalho do arquivo existe uma linha de comentário,
          fgets (readChars, 255, fd);  // ela começa com o char "#".

          fscanf (fd, "%s", readChars); // Pular a linha de comentário e ler a próxima linha.
        }
        sscanf (readChars, "%d", &pPgm->w);  // para preencher os campos da struct é importante manter a
        fscanf (fd, "%d", &pPgm->h);        // mesma ordem da definição desses campos.
        fscanf (fd, "%d", &pPgm->max);

         unsigned int Totalpixels = (pPgm->w) * (pPgm->h);

         pPgm->pData = (Tpixel*)malloc(sizeof(Tpixel) * (Totalpixels)); // pData é um vetor alocado dinamicamente
        // ler cada o valor de brilho de cada pixel do arquivo da imagem e armazená-lo corretamente no
        // suporte do reticulado (matriz) da imagem, o qual é um vetor criado dinamicamente.


        for (nRead = 0; nRead < Totalpixels; nRead++) {
          fscanf(fd, "%d" ,&word); // lê o valor de brilho de um pixel do arquivo
          pPgm->pData[nRead] = word; // armazena o valor deste pixel no vetor alocado
        } // for

       fclose(fd); // sempre fechar o arquivo se não é o mesmo não é mais necessário
printf("\n ..... (LIDOS DADOS do ARQUIVO )-->%s.", arq);

       return 0; // retorne os dados de imagem no nome da função
     } // ReadPGMP2 - fim da função


int MemImgZero(pgm* atual, pgm* zeropgm) {
    /// MemImgZero ---> cria na memoria RAM um reticulado para suporte de dados de imagem (pPgm)
    ///                 com dimensoes (pPgm->h x pPgm->w) = tp bytes. Todos esses  tp pixels do reticulado
    ///                 serao associados com o valor ZERO. O reticulado é um vetor de Tpixel
    ///                 apontado pelo ponteiro *pData (Tpixel *pData).
    ///                 Os dados sao copiados de pgm* atual
    zeropgm->w = atual->w;
    zeropgm->h = atual->h;
    zeropgm->max = atual->max;

    unsigned int tp, Totalpixels = (zeropgm->w) * (zeropgm->h);

    zeropgm->pData = (Tpixel*)malloc(sizeof(Tpixel) * (Totalpixels));
    /// pData é um vetor alocado dinamicamente.
    /// pData é o suporte do reticulado (matriz) da imagem.
    if (!(zeropgm->pData)) {
        printf("ERRO na alocacao de matriz de zeros.");
        return -1;
    }

    const Tpixel ZERO = 0;
    for (tp = 0; tp < Totalpixels; tp++) {
            zeropgm->pData[tp] = ZERO;
    } // for

printf("\n ..... Criada Matriz ZEROS[%d, %d](.", zeropgm->h, zeropgm->w);

    return 1; /// Sucesso
}


Tpixel GetPixel(pgm* img, int L, int C){

    /// abstrair o vetor img->pData como uma matriz
    /// de dimensoes  (img->h x img->w)

    if ((L >= img->h) ||  (C >= img->w) || ( L < 0) || (C < 0)) {
        printf("\n ..... Coordenada de imagem esta fora dos limites da grade da imagem.");
        exit(0);
    }
    Tpixel K;
    K = *((img->pData) + L*img->w + C);

    return (K);
}

int PutPixel(pgm* img, int L, int C, Tpixel v){

    /// abstrair o vetor img->pData como uma matriz
    /// de dimensoes  (img->h x img->w)

    if ((L >= img->h) ||  (C >= img->w) || ( L < 0) || (C < 0)) {
        printf("\n ..... Coordenada de imagem esta fora dos limites da grade da imagem.");
        exit(0);
    }
    Tpixel *K; ///unsigned long int *K;
   //*((img->pData) + L*img->w + C)
    K = (img->pData) + L*img->w + C;
   *K = v;

    return (1);
}


int MorphGrad33(pgm* img, pgm* grad) {
    /// computar o gradiente morfologico da imagem img.
    /// armazenar o resultado na imagem grad.
    MemImgZero(img, grad);

    /// fazer a convolucao da superficie da imagem com o
    /// elemento estruturante 3x3 (janela 3x3)

    int L, C, k, j;
    Tpixel max, p;
    for (L = 1; L < img->h - 1; L++) {
        for (C = 1; C < img->w - 1; C++) {
            max = 0;
            /// convolucao para dilatacao cinza 3x3
            for (k = -1; k<=1; k++) {    /// elemento estruturante EE 3x3
                for (j = -1; j<=1; j++){
                    p = GetPixel(img, L + k, C + j);
                    if (p > max) max = p;  /// valor de cinza maximo coberto pelo EE
                } // for j
            } // for k
            /// encontrou o valor de cinza maximo max coberto pelo elemento estruturante.
            /// armazenar o valor max nas coordenadas (L, C) da matrix grad.
            p = max - GetPixel(img, L , C);
            PutPixel(grad, L, C, p);
        } // for C

    } // for L
printf("\n ..... Processado o Gradiente Morfologico da Imagem.");
  return 0;
}


int main(){

    ///                 caminhos e NOMES dos arquivos em disco
    const char* NimgOR   = "../input/UmuNirGeoEye.pgm"; /// imagem original cinza.
    const char* NimgGROW = "UmuGrowGeoEye.pbm";/// imagem binária da silhueta do lago.
    const char* NimgBORD = "UmuBordGeoEye.pbm";///imagem binária da borda do lago.



    /// descritores para os dados das imagens na memória principal.
    /// estruturas de dados para armazenar dados de imagens como
    ///                    VETORES ou MATRIZES
    pgm *imgOR, *imgGROW, *imgBORD;

    /// alocação dos descritores de cada imagem

    imgOR = malloc (sizeof(pgm));///descritor para dados da imagem original.
    imgGROW = malloc (sizeof(pgm));///descritor para dados da imagem da silhueta do lago.
    imgBORD = malloc (sizeof(pgm));///descritor para dados da imagem da borda do lago.


    ///      ( NOME,  DESCRITOR )
    ReadPGMP2(NimgOR, imgOR);  /// transferir do arquivo para a memória principal
                               /// os dados da da imagem original.

    MemImgZero(imgOR, imgGROW); /// faz uma cópia da estrutura do descritor
          ///  da imagem imgOR para o descritor imgGROW e aloca todos os pixels
          ///  de  imgGROW->pData com o valor ZERO

    int critsim = 10;   /// limite do critério de similaridade.
    int LS = 377, CS = 507; /// coordenada linha (LS) e coluna (CS) do ponto semente.
    int VS = GetPixel(imgOR, LS, CS); /// VS - valor de brilho do ponto semente.

    /// ---------------------------------------------------
    /// Crescimento de Região - propagação do ponto semente
    /// ---------------------------------------------------

    ///                 CRIAR PILHA

    TpPILHA *pilha;
    TpDADO  x, y;

    /// alocar o descritor da PILHA.
    pilha = (TpPILHA*) malloc(sizeof(TpPILHA));
    if (!pilha) {
        printf("\n ERRO de alocacao da PILHA:\n");
        exit(0);
    }

    /// iniciar o TOPO da pilha com o valor NULL.
    InicializaPilha(pilha); /// indica que a pilha está vazia.

    x.L = LS; /// copiando o valor das coordenadas do ponto semente para os
    x.C = CS; /// campos da struct TpDADO usada no empilhamento das coordenadas.
    printf("\n Valor do Ponto Semente  =  %d \n", VS);

    /// empilhar a coordenada do ponto semente na pilha.
    push(x, pilha);
    /// marcar na imagem resultado que o ponto semente pertence a região desejada
    /// (tem o mesmo significado que fazer  imgGROW[x.L][x.C] = 1)
    PutPixel(imgGROW, x.L, x.C, 1);

    int k, j; /// auxiliares para visitar os 8-vizinhos de um pixel.
    int sim;  /// medida de similaridade de um pixel em relação ao valor VS do ponto semente.
    Tpixel Vpix; // auxiliar para valor de um pixel.
    Tpixel Growpix; // auxiliar para definir se um pixel já foi pocessado ou não.

    while (pilha->topo) {  /// enquanto existir coordenada na pilha,
        pop(&x, pilha);    /// desempilhar uma coordenada do topo
        for (k = -1; k<=1; k++) {    /// elemento estruturante EE 3x3
            y.L = x.L + k;  // só depende do k
            for (j = -1; j<=1; j++){
                y.C = x.C + j ;
                Vpix = GetPixel(imgOR, y.L, y.C); /// recuperar o valor do pixel (L,C)
                                             /// na imagem original imgOR
                Growpix = GetPixel(imgGROW, y.L, y.C); /// recuperar o valor do pixel (L,C)
                /// correspondente na imagem resultado imgGROW para saber se esse pixel já foi processado (1) ou não (0)
                sim = (abs(VS - Vpix) <= critsim);  
                /// Analisa se o valor do pixel Vpix é simeilar ao valor do ponto semente VS
                /// similar -> sim == 1, não similar -> sim == 0
        ///  Se esse pixel é similar (sim == 1) ao ponto semente e se ele NÃO foi processado (Growpix == 0):
        ///     (1)  marcar esse pixel na imagem resultado com o valor 1 e;
        ///     (2)  empilhar essa coordenada y.
        ///  A operação (1) diz que esse pixel pertence a região de interesse.
        ///  A operação (2) diz que todos os vizinhos desse pixel devem ser investigados.

                if (!Growpix && sim) {
                    push(y, pilha);  /// empilhar essa coordenada
                    PutPixel(imgGROW, y.L, y.C, 1); /// registrar o valor "1 o valor como resultado desejado.
                } // if
            } // for j
        } // for k
    } // while
    MorphGrad33(imgGROW, imgBORD);  /// gerar a borda da região

    WritePGM(NimgGROW, "P1", imgGROW);
    WritePGM(NimgBORD, "P1", imgBORD);

    /// gerar borda colorida - a estrutura de uma imagem colorida tem 3 vezes mais pixels.
                 //nome do arquivo da imagem colorida:
     const char* NColorBORDUmu  = "UmuColorBordGeoEye.ppm";
     pgm *imgCOLOR; // descritor da imagem colorida
     imgCOLOR = malloc (sizeof(pgm));///alocação do descritor para imagem colorida
     imgCOLOR->h = imgOR->h; /// o número de linhas é o mesmo. Entretanto,
     imgCOLOR->w = 3*(imgOR->w); /// o número de colunas na imagem colorida ("P3") é
                            /// três vezes maior que o número de colunas de uma imagem
                            /// em tons de cinza ("P2") ou binária ("P1"). O valor de
                            /// colunas 3 vezes maior que as outras imagens se deve ao
                            /// fato que agora precisamos de 3 valores (R, G, B) para cada
                            /// pixel da imagem.

    /// imgCOLOR->max = 255; -> Define o brilho máximo da imagem colorida como 255, mas não é necessário.

    /// Tem-se apenas o descritor da imagem imgCOLOR.
    /// Precisa-se alocar o reticulado de suporte da imagem (MATRIZ). 
    /// Uma maneira é copiar o descritor e criar uma imagens com pixels com valor zero.
    MemImgZero(imgCOLOR, imgCOLOR);  
    /// Com isso, a imagem imgCOLOR tem estrutura de uma imagem colorida e todos os pixels dela são pretos.

    /// Percorrer os pixels da imagem que contém somente a borda do objeto desejado.
    int TotalPix = imgBORD->w * imgBORD->h;
    j = -1;
    for (k = 0; k<TotalPix; k++){
        if (imgBORD->pData[k]) { /// se o k-ésimo pixel da borda é "1"
                /// mostra ele na cor vermelha na imagem colorida.
            imgCOLOR->pData[++j] = 255; /// R = 255 (Destaca o vermelho)
            imgCOLOR->pData[++j] = 0; /// G = 0 (desliga o verde)
            imgCOLOR->pData[++j] = 0; /// B = 0 (desliga o azul)
        } else { /// se não é borda, mostrar os brilhos de cinzas originais.
            imgCOLOR->pData[++j] = imgOR->pData[k];  /// repetir o o mesmo valor de
            imgCOLOR->pData[++j] = imgOR->pData[k];  /// cinza para completar os valores
            imgCOLOR->pData[++j] = imgOR->pData[k];  /// RGB dos pixels da imagem colorida
        }

    } // for
     WritePGM(NColorBORDUmu, "P3", imgCOLOR); /// gravar a imagem PPM ("P3")

    /// sequência correta de desalocação:
    /// (1) desalocar os dados (pixels) da imagem.
    /// (2) desalocar o descritos da imagem.

    ///  DESALOCAÇÃO DE MEMÓRIA
    
    ///  pixels da imagem    descritor da imagem
        free(imgOR->pData);      free(imgOR);
        free(imgGROW->pData);    free(imgGROW);
        free(imgBORD->pData);    free(imgBORD);
        free(imgCOLOR->pData);   free(imgCOLOR);

    /// desalocar o descritor da PILHA
       free(pilha);

return 0;
} // main
