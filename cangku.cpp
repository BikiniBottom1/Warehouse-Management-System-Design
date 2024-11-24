#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50

// 商品信息结构体
typedef struct Product {
    char* name;
    int quantity;
    int monthlySales;
    struct Product* next;
    int lowerThreshold;  // 下限警戒线
    int upperThreshold;  // 上限警戒线
} Product;

// 仓库信息结构体
typedef struct Warehouse {
    int id;
    char* name;
    struct Warehouse* next;
    Product* products;
} Warehouse;

// 全局变量，指向当前选中的仓库
Warehouse* currentWarehouse = NULL;
// 全局变量，指向仓库链表的头部
Warehouse* warehouseList = NULL;

// 创建新的仓库（没有商品信息）
void createWarehouse(int id, const char* name) {
    Warehouse* newWarehouse = (Warehouse*)malloc(sizeof(Warehouse));
    newWarehouse->name = (char*)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(newWarehouse->name, name);
    newWarehouse->id = id;
    newWarehouse->next = NULL;
    newWarehouse->products = NULL;

    // 将新仓库插入到链表尾部
    if (warehouseList == NULL) {
        warehouseList = newWarehouse;
        currentWarehouse = newWarehouse;  // 初始时，默认当前选中的仓库为新仓库
    } else {
        Warehouse* temp = warehouseList;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newWarehouse;
    }
    printf("创建仓库成功：%d - %s\n", id, name);
}

// 切换当前选中的仓库
void switchWarehouse(int warehouseId) {
    Warehouse* temp = warehouseList;
    while (temp != NULL) {
        if (temp->id == warehouseId) {
            currentWarehouse = temp;
            printf("已切换到仓库：%d - %s\n", warehouseId, temp->name);
            return;
        }
        temp = temp->next;
    }

    printf("未找到仓库：%d\n", warehouseId);
}

// 根据仓库 ID 查找仓库
Warehouse* findWarehouse(int id) {
    Warehouse* currentWarehouse = warehouseList;
    while (currentWarehouse != NULL) {
        if (currentWarehouse->id == id) {
            return currentWarehouse;
        }
        currentWarehouse = currentWarehouse->next;
    }
    return NULL;  // 未找到对应的仓库
}

// 根据商品名称查找商品
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
    return NULL;  // 未找到对应的商品
}

// 插入商品信息
void insertProduct(const char* name, int quantity, int monthlySales, int lowerThreshold, int upperThreshold) {
    Product* newProduct = (Product*)malloc(sizeof(Product));
    newProduct->name = (char*)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(newProduct->name, name);
    newProduct->quantity = quantity;
    newProduct->monthlySales = monthlySales;
    newProduct->lowerThreshold = lowerThreshold;
    newProduct->upperThreshold = upperThreshold;
    newProduct->next = NULL;

    // 将新商品插入到链表尾部
    if (currentWarehouse->products == NULL) {
        currentWarehouse->products = newProduct;
    } else {
        Product* temp = currentWarehouse->products;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newProduct;
    }
    printf("插入商品成功：%s\n", name);
}

// 删除商品信息
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
                printf("删除商品成功：%s\n", name);
                return;
            }

            prev = current;
            current = current->next;
        }

        tempWarehouse = tempWarehouse->next;
    }

    printf("未找到商品：%s\n", name);
}

// 查询商品信息（包括所在的仓库）
void searchProduct(const char* name) {
    Warehouse* tempWarehouse = warehouseList;
    int found = 0; // 用于标记是否找到商品

    while (tempWarehouse != NULL) {
        Product* currentProduct = tempWarehouse->products;
        while (currentProduct != NULL) {
            if (strstr(currentProduct->name, name) != NULL) {
                printf("所属仓库：%d - %s\n", tempWarehouse->id, tempWarehouse->name);
                printf("商品名：%s，数量：%d，月销量：%d\n", currentProduct->name, currentProduct->quantity, currentProduct->monthlySales);

                // 检查库存是否接近或者超出警戒线
                if (currentProduct->lowerThreshold != -1 && currentProduct->quantity <= currentProduct->lowerThreshold) {
                    printf("警告：%s 库存不足，请及时补货！\n", currentProduct->name);
                }
                if (currentProduct->upperThreshold != -1 && currentProduct->quantity >= currentProduct->upperThreshold) {
                    printf("警告：%s 库存过高，请注意管理库存！\n", currentProduct->name);
                }

                found = 1;
            }
            currentProduct = currentProduct->next;
        }
        tempWarehouse = tempWarehouse->next;
    }

    if (!found) {
        printf("未找到商品：%s\n", name);
    }
}

// 更新商品信息
void updateProduct(const char* name, int quantity, int monthlySales) {
    // 检查商品是否存在
    Product* existingProduct = findProduct(name);
    if (existingProduct == NULL) {
        printf("未找到商品：%s\n", name);
        return;
    }

    // 商品存在，进行更新
    // 检查库存是否接近或者超出警戒线
    if (existingProduct->lowerThreshold != -1 && quantity <= existingProduct->lowerThreshold) {
        printf("警告：%s 库存不足，请及时补货！\n", existingProduct->name);
    }
    if (existingProduct->upperThreshold != -1 && quantity >= existingProduct->upperThreshold) {
        printf("警告：%s 库存过高，请注意管理库存！\n", existingProduct->name);
    }

    existingProduct->quantity = quantity;
    existingProduct->monthlySales = monthlySales;
    printf("更新商品成功：%s\n", name);
}

// 快速排序辅助函数
Product* partition(Product* arr, int low, int high) {
    Product* pivot = &arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j].monthlySales > pivot->monthlySales) {
            i++;
            // 交换节点数据
            Product temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    // 交换基准元素到正确的位置
    Product temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return &arr[i + 1];
}

// 快速排序
void quickSortHelper(Product* arr, int low, int high) {
    if (low < high) {
        Product* partitionIndex = partition(arr, low, high);

        // 递归调用快速排序
        quickSortHelper(arr, low, partitionIndex - arr - 1);
        quickSortHelper(arr, partitionIndex - arr + 1, high);
    }
}

// 对所有商品的月销售量进行排序（快速排序）
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
        printf("没有商品信息\n");
        return;
    }

    // 将商品节点存储到数组中
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

    // 调用快速排序算法进行排序
    quickSortHelper(arr, 0, count - 1);

    printf("排序后的商品信息：\n");
    for (int i = 0; i < count; i++) {
        printf("商品名：%-10s 数量：%d 月销量：%d\n", arr[i].name, arr[i].quantity, arr[i].monthlySales);
    }

    free(arr);
}
// 统计每类商品的月销售量
void calculateAverageSales() {
     if (currentWarehouse == NULL || currentWarehouse->products == NULL) {
        printf("当前仓库没有商品信息\n");
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
        printf("当前仓库中商品的月平均销售量：%.2f\n", averageSales);
    } else {
        printf("当前仓库没有商品信息\n");
    }
}
// 保存仓库文件
void saveWarehouseFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法打开文件：%s\n", filename);
        return;
    }

    Warehouse* currentWarehouse = warehouseList;

    while (currentWarehouse != NULL) {
        fprintf(file, "仓库：%d - %s\n", currentWarehouse->id, currentWarehouse->name);

        Product* currentProduct = currentWarehouse->products;
        while (currentProduct != NULL) {
            fprintf(file, "%s %d %d %d %d\n", currentProduct->name, currentProduct->quantity, currentProduct->monthlySales, currentProduct->lowerThreshold, currentProduct->upperThreshold);
            currentProduct = currentProduct->next;
        }

        currentWarehouse = currentWarehouse->next;
    }

    fclose(file);
    printf("成功保存仓库文件：%s\n", filename);
}

// 加载仓库文件
void loadWarehouseFile(char filename[]) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开文件：%s\n", filename);
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "仓库：", strlen("仓库：")) == 0) {
            int id;
            char name[MAX_NAME_LENGTH];
            sscanf(line, "仓库：%d - %[^\n]", &id, name);
            
            // 判断仓库是否已经存在
            Warehouse* existingWarehouse = findWarehouse(id);
            if (existingWarehouse == NULL) {
                createWarehouse(id, name);
            }
        } else {
            char name[MAX_NAME_LENGTH];
            int quantity, monthlySales, lowerThreshold, upperThreshold;
            sscanf(line, "%s %d %d %d %d", name, &quantity, &monthlySales, &lowerThreshold, &upperThreshold);
            
            // 判断商品是否已经存在
            Product* existingProduct = findProduct(name);
            if (existingProduct == NULL) {
                insertProduct(name, quantity, monthlySales, lowerThreshold, upperThreshold);
            } else {
                // 商品已存在，进行更新操作
                existingProduct->quantity = quantity;
                existingProduct->monthlySales = monthlySales;
                existingProduct->lowerThreshold = lowerThreshold;
                existingProduct->upperThreshold = upperThreshold;
            }
        }
    }

    fclose(file);
    printf("成功读取仓库文件：%s\n", filename);

    // 显示仓库和商品信息
    printf("仓库商品信息：\n");
    Warehouse* tempWarehouse = warehouseList;
    while (tempWarehouse != NULL) {
        printf("仓库：%d - %s\n", tempWarehouse->id, tempWarehouse->name);

        Product* currentProduct = tempWarehouse->products;
        while (currentProduct != NULL) {
            printf("商品名：%s，数量：%d，月销量：%d\n", currentProduct->name, currentProduct->quantity, currentProduct->monthlySales);
            currentProduct = currentProduct->next;
        }

        tempWarehouse = tempWarehouse->next;
        if (tempWarehouse != NULL) {
            printf("\n");  // 添加换行符以区分不同仓库的商品信息
        }
    }
}


// 设置商品警戒线
void setThreshold(const char* name, int lowerThreshold, int upperThreshold) {
    Warehouse* currentWarehouse = warehouseList;
    int found = 0;

    while (currentWarehouse != NULL) {
        Product* current = currentWarehouse->products;

        while (current != NULL) {
            if (strcmp(current->name, name) == 0) {
                current->lowerThreshold = lowerThreshold;
                current->upperThreshold = upperThreshold;
                printf("已设置商品警戒线：%s，下限：%d，上限：%d\n", name, lowerThreshold, upperThreshold);
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
        printf("未找到商品：%s\n", name);
    }
}

void test() {
    createWarehouse(1, "香水香氛");
    switchWarehouse(1);
    insertProduct("范思哲晶钻香水", 100, 50, 10, 150);
    insertProduct("经典古龙水", 200, 80, 20, 210);
    insertProduct("柏氛101香水", 150, 30, 5, 50);
    printf("\n");

    createWarehouse(2, "零食饮料");
    switchWarehouse(2);
    insertProduct("饮用水", 300, 100, 50, 500);
    insertProduct("东方树叶", 500, 200, 100, 800);
    insertProduct("健达奇趣蛋", 400, 150, 80, 600);
}

int main() {
    test();

    int choice;
    char productName[MAX_NAME_LENGTH];
    char warehouseName[MAX_NAME_LENGTH];
    int warehouseId, quantity, monthlySales, lowerThreshold, upperThreshold;

    while (1) {
        printf("\n--- 仓库管理系统 ---\n");
        printf("1. 创建新仓库\n");
        printf("2. 切换仓库\n");
        printf("3. 插入商品\n");
        printf("4. 删除商品\n");
        printf("5. 查询商品\n");
        printf("6. 更新商品\n");
        printf("7. 对所有仓库的商品月销售量排序\n");
        printf("8. 统计每类商品的月平均销售量\n");
        printf("9. 读取仓库文件\n");
        printf("10. 保存仓库文件\n");
        printf("11. 设置商品警戒线\n");
        printf("0. 退出\n");
        printf("请选择操作：");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("请输入新仓库编号：");
                scanf("%d", &warehouseId);
                printf("请输入新仓库名称：");
                scanf("%s", warehouseName);
                createWarehouse(warehouseId, warehouseName);
                break;
            case 2:
                printf("请输入要切换的仓库编号：");
                scanf("%d", &warehouseId);
                switchWarehouse(warehouseId);
                break;
            case 3:
                printf("请输入商品名称：");
                scanf("%s", productName);
                printf("请输入商品数量：");
                scanf("%d", &quantity);
                printf("请输入商品月销量：");
                scanf("%d", &monthlySales);
                printf("请输入商品下限警戒线（-1 表示无）：");
                scanf("%d", &lowerThreshold);
                printf("请输入商品上限警戒线（-1 表示无）：");
                scanf("%d", &upperThreshold);
                insertProduct(productName, quantity, monthlySales, lowerThreshold, upperThreshold);
                break;
            case 4:
                printf("请输入要删除的商品名称：");
                scanf("%s", productName);
                deleteProduct(productName);
                break;
            case 5:
                printf("请输入要查询的商品名称关键字：");
                scanf("%s", productName);
                searchProduct(productName);
                break;
            case 6:
                printf("请输入要更新的商品名称：");
                scanf("%s", productName);
                printf("请输入新的商品数量：");
                scanf("%d", &quantity);
                printf("请输入新的商品月销量：");
                scanf("%d", &monthlySales);
                updateProduct(productName, quantity, monthlySales);
                break;
            case 7:
                sortProductsBySales();
                printf("已对所有仓库的商品月销售量排序\n");
                break;
            case 8:
                calculateAverageSales();
                break;
            case 9:
                printf("请输入要读取的仓库文件名：");
                scanf("%s", warehouseName);
                loadWarehouseFile(warehouseName);
                break;
            case 10:
                printf("请输入要保存的仓库文件名：");
                scanf("%s", warehouseName);
                saveWarehouseFile(warehouseName);
                break;
            case 11:
                printf("请输入要设置警戒线的商品名称：");
                scanf("%s", productName);
                printf("请输入下限警戒线（输入 -1 表示无）：");
                scanf("%d", &lowerThreshold);
                printf("请输入上限警戒线（输入 -1 表示无）：");
                scanf("%d", &upperThreshold);
                setThreshold(productName, lowerThreshold, upperThreshold);
                break;
            case 0:
                printf("感谢使用，再见！\n");
                exit(0);
            default:
                printf("无效的选择，请重新输入\n");
                break;
        }
    }

    return 0;
}

