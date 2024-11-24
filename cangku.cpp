#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50

// ��Ʒ��Ϣ�ṹ��
typedef struct Product {
    char* name;
    int quantity;
    int monthlySales;
    struct Product* next;
    int lowerThreshold;  // ���޾�����
    int upperThreshold;  // ���޾�����
} Product;

// �ֿ���Ϣ�ṹ��
typedef struct Warehouse {
    int id;
    char* name;
    struct Warehouse* next;
    Product* products;
} Warehouse;

// ȫ�ֱ�����ָ��ǰѡ�еĲֿ�
Warehouse* currentWarehouse = NULL;
// ȫ�ֱ�����ָ��ֿ������ͷ��
Warehouse* warehouseList = NULL;

// �����µĲֿ⣨û����Ʒ��Ϣ��
void createWarehouse(int id, const char* name) {
    Warehouse* newWarehouse = (Warehouse*)malloc(sizeof(Warehouse));
    newWarehouse->name = (char*)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(newWarehouse->name, name);
    newWarehouse->id = id;
    newWarehouse->next = NULL;
    newWarehouse->products = NULL;

    // ���²ֿ���뵽����β��
    if (warehouseList == NULL) {
        warehouseList = newWarehouse;
        currentWarehouse = newWarehouse;  // ��ʼʱ��Ĭ�ϵ�ǰѡ�еĲֿ�Ϊ�²ֿ�
    } else {
        Warehouse* temp = warehouseList;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newWarehouse;
    }
    printf("�����ֿ�ɹ���%d - %s\n", id, name);
}

// �л���ǰѡ�еĲֿ�
void switchWarehouse(int warehouseId) {
    Warehouse* temp = warehouseList;
    while (temp != NULL) {
        if (temp->id == warehouseId) {
            currentWarehouse = temp;
            printf("���л����ֿ⣺%d - %s\n", warehouseId, temp->name);
            return;
        }
        temp = temp->next;
    }

    printf("δ�ҵ��ֿ⣺%d\n", warehouseId);
}

// ���ݲֿ� ID ���Ҳֿ�
Warehouse* findWarehouse(int id) {
    Warehouse* currentWarehouse = warehouseList;
    while (currentWarehouse != NULL) {
        if (currentWarehouse->id == id) {
            return currentWarehouse;
        }
        currentWarehouse = currentWarehouse->next;
    }
    return NULL;  // δ�ҵ���Ӧ�Ĳֿ�
}

// ������Ʒ���Ʋ�����Ʒ
Product* findProduct(const char* name) {
    Warehouse* currentWarehouse = warehouseList;
    while (currentWarehouse != NULL) {
        Product* currentProduct = currentWarehouse->products;
        while (currentProduct != NULL) {
            if (strcmp(currentProduct->name, name) == 0) {
                return currentProduct;
            }
            currentProduct = currentProduct->next;
        }
        currentWarehouse = currentWarehouse->next;
    }
    return NULL;  // δ�ҵ���Ӧ����Ʒ
}

// ������Ʒ��Ϣ
void insertProduct(const char* name, int quantity, int monthlySales, int lowerThreshold, int upperThreshold) {
    Product* newProduct = (Product*)malloc(sizeof(Product));
    newProduct->name = (char*)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(newProduct->name, name);
    newProduct->quantity = quantity;
    newProduct->monthlySales = monthlySales;
    newProduct->lowerThreshold = lowerThreshold;
    newProduct->upperThreshold = upperThreshold;
    newProduct->next = NULL;

    // ������Ʒ���뵽����β��
    if (currentWarehouse->products == NULL) {
        currentWarehouse->products = newProduct;
    } else {
        Product* temp = currentWarehouse->products;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newProduct;
    }
    printf("������Ʒ�ɹ���%s\n", name);
}

// ɾ����Ʒ��Ϣ
void deleteProduct(const char* name) {
    Warehouse* tempWarehouse = warehouseList;
    while (tempWarehouse != NULL) {
        Product* prev = NULL;
        Product* current = tempWarehouse->products;

        while (current != NULL) {
            if (strcmp(current->name, name) == 0) {
                if (prev == NULL) {
                    tempWarehouse->products = current->next;
                } else {
                    prev->next = current->next;
                }
                free(current->name);
                free(current);
                printf("ɾ����Ʒ�ɹ���%s\n", name);
                return;
            }

            prev = current;
            current = current->next;
        }

        tempWarehouse = tempWarehouse->next;
    }

    printf("δ�ҵ���Ʒ��%s\n", name);
}

// ��ѯ��Ʒ��Ϣ���������ڵĲֿ⣩
void searchProduct(const char* name) {
    Warehouse* tempWarehouse = warehouseList;
    int found = 0; // ���ڱ���Ƿ��ҵ���Ʒ

    while (tempWarehouse != NULL) {
        Product* currentProduct = tempWarehouse->products;
        while (currentProduct != NULL) {
            if (strstr(currentProduct->name, name) != NULL) {
                printf("�����ֿ⣺%d - %s\n", tempWarehouse->id, tempWarehouse->name);
                printf("��Ʒ����%s��������%d����������%d\n", currentProduct->name, currentProduct->quantity, currentProduct->monthlySales);

                // ������Ƿ�ӽ����߳���������
                if (currentProduct->lowerThreshold != -1 && currentProduct->quantity <= currentProduct->lowerThreshold) {
                    printf("���棺%s ��治�㣬�뼰ʱ������\n", currentProduct->name);
                }
                if (currentProduct->upperThreshold != -1 && currentProduct->quantity >= currentProduct->upperThreshold) {
                    printf("���棺%s �����ߣ���ע������棡\n", currentProduct->name);
                }

                found = 1;
            }
            currentProduct = currentProduct->next;
        }
        tempWarehouse = tempWarehouse->next;
    }

    if (!found) {
        printf("δ�ҵ���Ʒ��%s\n", name);
    }
}

// ������Ʒ��Ϣ
void updateProduct(const char* name, int quantity, int monthlySales) {
    // �����Ʒ�Ƿ����
    Product* existingProduct = findProduct(name);
    if (existingProduct == NULL) {
        printf("δ�ҵ���Ʒ��%s\n", name);
        return;
    }

    // ��Ʒ���ڣ����и���
    // ������Ƿ�ӽ����߳���������
    if (existingProduct->lowerThreshold != -1 && quantity <= existingProduct->lowerThreshold) {
        printf("���棺%s ��治�㣬�뼰ʱ������\n", existingProduct->name);
    }
    if (existingProduct->upperThreshold != -1 && quantity >= existingProduct->upperThreshold) {
        printf("���棺%s �����ߣ���ע������棡\n", existingProduct->name);
    }

    existingProduct->quantity = quantity;
    existingProduct->monthlySales = monthlySales;
    printf("������Ʒ�ɹ���%s\n", name);
}

// ��������������
Product* partition(Product* arr, int low, int high) {
    Product* pivot = &arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j].monthlySales > pivot->monthlySales) {
            i++;
            // �����ڵ�����
            Product temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    // ������׼Ԫ�ص���ȷ��λ��
    Product temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return &arr[i + 1];
}

// ��������
void quickSortHelper(Product* arr, int low, int high) {
    if (low < high) {
        Product* partitionIndex = partition(arr, low, high);

        // �ݹ���ÿ�������
        quickSortHelper(arr, low, partitionIndex - arr - 1);
        quickSortHelper(arr, partitionIndex - arr + 1, high);
    }
}

// ��������Ʒ�����������������򣨿�������
void sortProductsBySales() {
    int count = 0;
    Warehouse* tempWarehouse = warehouseList;

    while (tempWarehouse != NULL) {
        Product* current = tempWarehouse->products;
        while (current != NULL) {
            count++;
            current = current->next;
        }
        tempWarehouse = tempWarehouse->next;
    }

    if (count == 0) {
        printf("û����Ʒ��Ϣ\n");
        return;
    }

    // ����Ʒ�ڵ�洢��������
    Product* arr = (Product*)malloc(count * sizeof(Product));
    tempWarehouse = warehouseList;
    int index = 0;

    while (tempWarehouse != NULL) {
        Product* current = tempWarehouse->products;
        while (current != NULL) {
            arr[index] = *current;
            current = current->next;
            index++;
        }
        tempWarehouse = tempWarehouse->next;
    }

    // ���ÿ��������㷨��������
    quickSortHelper(arr, 0, count - 1);

    printf("��������Ʒ��Ϣ��\n");
    for (int i = 0; i < count; i++) {
        printf("��Ʒ����%-10s ������%d ��������%d\n", arr[i].name, arr[i].quantity, arr[i].monthlySales);
    }

    free(arr);
}
// ͳ��ÿ����Ʒ����������
void calculateAverageSales() {
     if (currentWarehouse == NULL || currentWarehouse->products == NULL) {
        printf("��ǰ�ֿ�û����Ʒ��Ϣ\n");
        return;
    }

    int count = 0;
    int totalSales = 0;
    Product* currentProduct = currentWarehouse->products;

    while (currentProduct != NULL) {
        totalSales += currentProduct->monthlySales;
        count++;
        currentProduct = currentProduct->next;
    }

    if (count > 0) {
        float averageSales = (float)totalSales / count;
        printf("��ǰ�ֿ�����Ʒ����ƽ����������%.2f\n", averageSales);
    } else {
        printf("��ǰ�ֿ�û����Ʒ��Ϣ\n");
    }
}
// ����ֿ��ļ�
void saveWarehouseFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("�޷����ļ���%s\n", filename);
        return;
    }

    Warehouse* currentWarehouse = warehouseList;

    while (currentWarehouse != NULL) {
        fprintf(file, "�ֿ⣺%d - %s\n", currentWarehouse->id, currentWarehouse->name);

        Product* currentProduct = currentWarehouse->products;
        while (currentProduct != NULL) {
            fprintf(file, "%s %d %d %d %d\n", currentProduct->name, currentProduct->quantity, currentProduct->monthlySales, currentProduct->lowerThreshold, currentProduct->upperThreshold);
            currentProduct = currentProduct->next;
        }

        currentWarehouse = currentWarehouse->next;
    }

    fclose(file);
    printf("�ɹ�����ֿ��ļ���%s\n", filename);
}

// ���زֿ��ļ�
void loadWarehouseFile(char filename[]) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("�޷����ļ���%s\n", filename);
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "�ֿ⣺", strlen("�ֿ⣺")) == 0) {
            int id;
            char name[MAX_NAME_LENGTH];
            sscanf(line, "�ֿ⣺%d - %[^\n]", &id, name);
            
            // �жϲֿ��Ƿ��Ѿ�����
            Warehouse* existingWarehouse = findWarehouse(id);
            if (existingWarehouse == NULL) {
                createWarehouse(id, name);
            }
        } else {
            char name[MAX_NAME_LENGTH];
            int quantity, monthlySales, lowerThreshold, upperThreshold;
            sscanf(line, "%s %d %d %d %d", name, &quantity, &monthlySales, &lowerThreshold, &upperThreshold);
            
            // �ж���Ʒ�Ƿ��Ѿ�����
            Product* existingProduct = findProduct(name);
            if (existingProduct == NULL) {
                insertProduct(name, quantity, monthlySales, lowerThreshold, upperThreshold);
            } else {
                // ��Ʒ�Ѵ��ڣ����и��²���
                existingProduct->quantity = quantity;
                existingProduct->monthlySales = monthlySales;
                existingProduct->lowerThreshold = lowerThreshold;
                existingProduct->upperThreshold = upperThreshold;
            }
        }
    }

    fclose(file);
    printf("�ɹ���ȡ�ֿ��ļ���%s\n", filename);

    // ��ʾ�ֿ����Ʒ��Ϣ
    printf("�ֿ���Ʒ��Ϣ��\n");
    Warehouse* tempWarehouse = warehouseList;
    while (tempWarehouse != NULL) {
        printf("�ֿ⣺%d - %s\n", tempWarehouse->id, tempWarehouse->name);

        Product* currentProduct = tempWarehouse->products;
        while (currentProduct != NULL) {
            printf("��Ʒ����%s��������%d����������%d\n", currentProduct->name, currentProduct->quantity, currentProduct->monthlySales);
            currentProduct = currentProduct->next;
        }

        tempWarehouse = tempWarehouse->next;
        if (tempWarehouse != NULL) {
            printf("\n");  // ��ӻ��з������ֲ�ͬ�ֿ����Ʒ��Ϣ
        }
    }
}


// ������Ʒ������
void setThreshold(const char* name, int lowerThreshold, int upperThreshold) {
    Warehouse* currentWarehouse = warehouseList;
    int found = 0;

    while (currentWarehouse != NULL) {
        Product* current = currentWarehouse->products;

        while (current != NULL) {
            if (strcmp(current->name, name) == 0) {
                current->lowerThreshold = lowerThreshold;
                current->upperThreshold = upperThreshold;
                printf("��������Ʒ�����ߣ�%s�����ޣ�%d�����ޣ�%d\n", name, lowerThreshold, upperThreshold);
                found = 1;
                break;
            }
            current = current->next;
        }

        if (found) {
            break;
        }

        currentWarehouse = currentWarehouse->next;
    }

    if (!found) {
        printf("δ�ҵ���Ʒ��%s\n", name);
    }
}

void test() {
    createWarehouse(1, "��ˮ���");
    switchWarehouse(1);
    insertProduct("��˼�ܾ�����ˮ", 100, 50, 10, 150);
    insertProduct("�������ˮ", 200, 80, 20, 210);
    insertProduct("�ط�101��ˮ", 150, 30, 5, 50);
    printf("\n");

    createWarehouse(2, "��ʳ����");
    switchWarehouse(2);
    insertProduct("����ˮ", 300, 100, 50, 500);
    insertProduct("������Ҷ", 500, 200, 100, 800);
    insertProduct("������Ȥ��", 400, 150, 80, 600);
}

int main() {
    test();

    int choice;
    char productName[MAX_NAME_LENGTH];
    char warehouseName[MAX_NAME_LENGTH];
    int warehouseId, quantity, monthlySales, lowerThreshold, upperThreshold;

    while (1) {
        printf("\n--- �ֿ����ϵͳ ---\n");
        printf("1. �����²ֿ�\n");
        printf("2. �л��ֿ�\n");
        printf("3. ������Ʒ\n");
        printf("4. ɾ����Ʒ\n");
        printf("5. ��ѯ��Ʒ\n");
        printf("6. ������Ʒ\n");
        printf("7. �����вֿ����Ʒ������������\n");
        printf("8. ͳ��ÿ����Ʒ����ƽ��������\n");
        printf("9. ��ȡ�ֿ��ļ�\n");
        printf("10. ����ֿ��ļ�\n");
        printf("11. ������Ʒ������\n");
        printf("0. �˳�\n");
        printf("��ѡ�������");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("�������²ֿ��ţ�");
                scanf("%d", &warehouseId);
                printf("�������²ֿ����ƣ�");
                scanf("%s", warehouseName);
                createWarehouse(warehouseId, warehouseName);
                break;
            case 2:
                printf("������Ҫ�л��Ĳֿ��ţ�");
                scanf("%d", &warehouseId);
                switchWarehouse(warehouseId);
                break;
            case 3:
                printf("��������Ʒ���ƣ�");
                scanf("%s", productName);
                printf("��������Ʒ������");
                scanf("%d", &quantity);
                printf("��������Ʒ��������");
                scanf("%d", &monthlySales);
                printf("��������Ʒ���޾����ߣ�-1 ��ʾ�ޣ���");
                scanf("%d", &lowerThreshold);
                printf("��������Ʒ���޾����ߣ�-1 ��ʾ�ޣ���");
                scanf("%d", &upperThreshold);
                insertProduct(productName, quantity, monthlySales, lowerThreshold, upperThreshold);
                break;
            case 4:
                printf("������Ҫɾ������Ʒ���ƣ�");
                scanf("%s", productName);
                deleteProduct(productName);
                break;
            case 5:
                printf("������Ҫ��ѯ����Ʒ���ƹؼ��֣�");
                scanf("%s", productName);
                searchProduct(productName);
                break;
            case 6:
                printf("������Ҫ���µ���Ʒ���ƣ�");
                scanf("%s", productName);
                printf("�������µ���Ʒ������");
                scanf("%d", &quantity);
                printf("�������µ���Ʒ��������");
                scanf("%d", &monthlySales);
                updateProduct(productName, quantity, monthlySales);
                break;
            case 7:
                sortProductsBySales();
                printf("�Ѷ����вֿ����Ʒ������������\n");
                break;
            case 8:
                calculateAverageSales();
                break;
            case 9:
                printf("������Ҫ��ȡ�Ĳֿ��ļ�����");
                scanf("%s", warehouseName);
                loadWarehouseFile(warehouseName);
                break;
            case 10:
                printf("������Ҫ����Ĳֿ��ļ�����");
                scanf("%s", warehouseName);
                saveWarehouseFile(warehouseName);
                break;
            case 11:
                printf("������Ҫ���þ����ߵ���Ʒ���ƣ�");
                scanf("%s", productName);
                printf("���������޾����ߣ����� -1 ��ʾ�ޣ���");
                scanf("%d", &lowerThreshold);
                printf("���������޾����ߣ����� -1 ��ʾ�ޣ���");
                scanf("%d", &upperThreshold);
                setThreshold(productName, lowerThreshold, upperThreshold);
                break;
            case 0:
                printf("��лʹ�ã��ټ���\n");
                exit(0);
            default:
                printf("��Ч��ѡ������������\n");
                break;
        }
    }

    return 0;
}

