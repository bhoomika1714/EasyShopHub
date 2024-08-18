////////////////////////////////////////////////////////////////////////////////////////////
//
//             Implemented in C Language  
//
///////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// User structure
typedef struct UserNode {
    char username[50];
    char password[50];
    struct UserNode* next;
} UserNode;

// Item structure
typedef struct ItemNode {
    char name[50];
    float price;
    int quantity;
    struct ItemNode* next;
} ItemNode;

// Order status history structure (Segment Tree)
typedef struct OrderStatusNode {
    char status[50];
    struct OrderStatusNode* left;
    struct OrderStatusNode* right;
} OrderStatusNode;

// Customer order history structure (Merge Sort)
typedef struct OrderHistoryNode {
    int orderID;
    char customerName[50];
    char orderDate[20];
    char orderTime[20];
    ItemNode itemNode; // Each OrderHistoryNode contains an ItemNode
    struct OrderHistoryNode* next;
} OrderHistoryNode;

// Function prototypes
UserNode* userSignup(UserNode* userList);
UserNode* userLogin(UserNode* userList);
OrderHistoryNode* storeItem(OrderHistoryNode* orderHistoryList, int* orderID);
void findItem(OrderHistoryNode* orderHistoryList);
void displayItems(OrderHistoryNode* orderHistoryList);
OrderStatusNode* updateOrderStatus(OrderStatusNode* root, char customerName[50], int orderID, char orderStatus[50]);
OrderHistoryNode* mergeSort(OrderHistoryNode* orderHistoryList);
void dfsOrderStatusTraversal(OrderStatusNode* node);

void dfsOrderTraversal(OrderHistoryNode* orderHistoryList);
void printOrderStatusHistory(OrderStatusNode* orderStatusHistory);
void freeMemory(UserNode* userList, OrderHistoryNode* orderHistoryList, OrderStatusNode* orderStatusHistory);


//  update order status history using a Segment Tree
OrderStatusNode* updateOrderStatus(OrderStatusNode* root, char customerName[50], int orderID, char orderStatus[50]) {
    OrderStatusNode* newNode = (OrderStatusNode*)malloc(sizeof(OrderStatusNode));
    strcpy(newNode->status, orderStatus);
    newNode->left = newNode->right = NULL;



    return newNode;
}

//  create a new user account
UserNode* userSignup(UserNode* userList) {
    UserNode* newUser = (UserNode*)malloc(sizeof(UserNode));
    if (newUser == NULL) {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter username: ");
    scanf("%s", newUser->username);
    printf("Enter password: ");
    scanf("%s", newUser->password);

    newUser->next = userList;
    return newUser;
}

//  check user  and perform login
UserNode* userLogin(UserNode* userList) {
    char username[50];
    char password[50];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    UserNode* current = userList;
    while (current != NULL) {
        if (strcmp(username, current->username) == 0 && strcmp(password, current->password) == 0) {
            printf("Login successful.\n");
            return userList;
        }
        current = current->next;
    }

    printf("Invalid username or password.\n");
    return userList;
}

//  store a new item
OrderHistoryNode* storeItem(OrderHistoryNode* orderHistoryList, int* orderID) {
    OrderHistoryNode* newOrder = (OrderHistoryNode*)malloc(sizeof(OrderHistoryNode));
    if (newOrder == NULL) {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter customer name: ");
    scanf("%s", newOrder->customerName);
    printf("Enter order date: ");
    scanf("%s", newOrder->orderDate);
    printf("Enter order time: ");
    scanf("%s", newOrder->orderTime);

    // Item information
    printf("Enter item name: ");
    scanf("%s", newOrder->itemNode.name);
    printf("Enter item price: ");
    scanf("%f", &newOrder->itemNode.price);
    printf("Enter item quantity: ");
    scanf("%d", &newOrder->itemNode.quantity);

    newOrder->orderID = (*orderID)++;
    newOrder->next = orderHistoryList;
    return newOrder;
}

//  display items that are placed
void displayItems(OrderHistoryNode* orderHistoryList) {
    if (orderHistoryList == NULL) {
        printf("No orders to display.\n");
        return;
    }

    printf("----- All Items -----\n");
    OrderHistoryNode* current = orderHistoryList;
    while (current != NULL) {
        printf("OrderID: %d, Customer: %s, Date: %s, Time: %s, Item: %s, Price: %.2f, Quantity: %d\n",
               current->orderID, current->customerName, current->orderDate, current->orderTime,
               current->itemNode.name, current->itemNode.price, current->itemNode.quantity);
        current = current->next;
    }
}

// Function to find a specific item
void findItem(OrderHistoryNode* orderHistoryList) {
    char itemName[50];
    printf("Enter the name of the item to find: ");
    scanf("%s", itemName);

    OrderHistoryNode* current = orderHistoryList;
    while (current != NULL) {
        if (strcmp(itemName, current->itemNode.name) == 0) {
            printf("Item found in OrderID %d: %s, Price: %.2f, Quantity: %d\n",
                   current->orderID, current->itemNode.name, current->itemNode.price, current->itemNode.quantity);
            return;
        }
        current = current->next;
    }

    printf("Item not found.\n");
}

//  sort customer order history using Merge Sort based on item price
OrderHistoryNode* merge(OrderHistoryNode* left, OrderHistoryNode* right) {
    if (left == NULL) return right;
    if (right == NULL) return left;

    //  (sort based on item price)
    if (left->itemNode.price <= right->itemNode.price) {
        left->next = merge(left->next, right);
        return left;
    } else {
        right->next = merge(left, right->next);
        return right;
    }
}

OrderHistoryNode* mergeSort(OrderHistoryNode* orderHistoryList) {
    if (orderHistoryList == NULL || orderHistoryList->next == NULL) {
        return orderHistoryList;
    }

    // Split the list into two halves
    OrderHistoryNode* slow = orderHistoryList;
    OrderHistoryNode* fast = orderHistoryList->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    OrderHistoryNode* left = orderHistoryList;
    OrderHistoryNode* right = slow->next;
    slow->next = NULL;

    // Recursively sort the two halves
    left = mergeSort(left);
    right = mergeSort(right);

    // Merge the sorted halves
    return merge(left, right);
}

//  DFS order traversal
void dfsOrderTraversal(OrderHistoryNode* orderHistoryList) {
    if (orderHistoryList == NULL) {
        printf("No orders to traverse.\n");
        return;
    }

    // Visited array to keep track of visited nodes
    int* visited = (int*)malloc(sizeof(int));
    for (OrderHistoryNode* current = orderHistoryList; current != NULL; current = current->next) {
        visited[current->orderID] = 0;
    }

    // DFS traversal
    for (OrderHistoryNode* current = orderHistoryList; current != NULL; current = current->next) {
        if (!visited[current->orderID]) {
            printf("OrderID: %d, Customer: %s, Date: %s, Time: %s, Item: %s, Price: %.2f, Quantity: %d\n",
                   current->orderID, current->customerName, current->orderDate, current->orderTime,
                   current->itemNode.name, current->itemNode.price, current->itemNode.quantity);
            visited[current->orderID] = 1;
        }
    }

    free(visited);
    printf("DFS Order Traversal completed.\n");
}

/*OrderHistoryNode* deleteItem(OrderHistoryNode* orderHistoryList, int orderID) {
    //  delete a specific item
    OrderHistoryNode* current = orderHistoryList;
    OrderHistoryNode* prev = NULL;

    while (current != NULL) {
        if (current->orderID == orderID) {
            if (prev == NULL) {
                orderHistoryList = current->next;
            } else {
                prev->next = current->next;
            }

            free(current);
            printf("Item with OrderID %d deleted.\n", orderID);
            return orderHistoryList;
        }

        prev = current;
        current = current->next;
    }

    printf("Item with OrderID %d not found.\n", orderID);
    return orderHistoryList;
}*/

OrderHistoryNode* deleteItemByOrderID(OrderHistoryNode* orderHistoryList, int orderID) {
    OrderHistoryNode* current = orderHistoryList;
    OrderHistoryNode* prev = NULL;

    while (current != NULL) {
        if (current->orderID == orderID) {
            if (prev == NULL) {
                orderHistoryList = current->next;
            } else {
                prev->next = current->next;
            }

            free(current);
            printf("Item with OrderID %d deleted.\n", orderID);
            return orderHistoryList;
        }

        prev = current;
        current = current->next;
    }

    printf("Item with OrderID %d not found.\n", orderID);
    return orderHistoryList;
}

void printOrderStatusHistory(OrderStatusNode* orderStatusHistory) {
    if (orderStatusHistory == NULL) {
        printf("No order status history available.\n");
        return;
    }

    printf("Order Status History:\n");
    dfsOrderStatusTraversal(orderStatusHistory);
}

//  DFS order status history traversal
void dfsOrderStatusTraversal(OrderStatusNode* node) {
    if (node != NULL) {
        dfsOrderStatusTraversal(node->left);
        printf("Status: %s\n", node->status);
        dfsOrderStatusTraversal(node->right);
    }
}


// Function to free allocated memory
void freeMemory(UserNode* userList, OrderHistoryNode* orderHistoryList, OrderStatusNode* orderStatusHistory) {
    // Free userList
    UserNode* tempUser;
    while (userList != NULL) {
        tempUser = userList;
        userList = userList->next;
        free(tempUser);
    }

    // Free orderHistoryList
    OrderHistoryNode* tempOrder;
    while (orderHistoryList != NULL) {
        tempOrder = orderHistoryList;
        orderHistoryList = orderHistoryList->next;
        free(tempOrder);
    }


}

int main() {
    UserNode* userList = NULL;
    OrderHistoryNode* orderHistoryList = NULL;
    OrderStatusNode* orderStatusHistory = NULL;

    int choice;
    int orderID = 1;

    do {
        printf("\n----- Menu -----\n");
        printf("1. User Signup\n");
        printf("2. User Login\n");
        printf("3. Store Item\n");
        printf("4. Find Item\n");
        printf("5. Display Items\n");
        printf("6. Update Order Status\n");
        printf("7. Sort Customer Order History\n");
        printf("8. DFS Order Traversal\n");
         printf("9. Delete Item\n");
        printf("10. Print Order Status History\n");
        printf("11. Free Memory and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);


        switch (choice) {
            case 1:
                userList = userSignup(userList);
                break;
            case 2:
                userList = userLogin(userList);
                break;
            case 3:
                // Add an order ID and increment for each new order
                orderHistoryList = storeItem(orderHistoryList, &orderID);
                break;
            case 4:
                findItem(orderHistoryList);
                break;
            case 5:
                displayItems(orderHistoryList);
                break;
            case 6:
                printf("Enter customer name: ");
                char customerName[50];
                scanf("%s", customerName);
                printf("Enter order ID: ");
                int inputOrderID;
                scanf("%d", &inputOrderID);
                printf("Enter order status: ");
                char orderStatus[50];
                scanf("%s", orderStatus);
                orderStatusHistory = updateOrderStatus(orderStatusHistory, customerName, inputOrderID, orderStatus);
                break;
            case 7:
                orderHistoryList = mergeSort(orderHistoryList);
                break;
            case 8:
                dfsOrderTraversal(orderHistoryList);
                break;
            case 9:
             printf("Enter order ID to delete: ");
    int deleteOrderID;
    scanf("%d", &deleteOrderID);
    orderHistoryList = deleteItemByOrderID(orderHistoryList, deleteOrderID);
    break;
               // orderHistoryList = deleteItem(orderHistoryList, orderID);
                //break;
            case 10:
                printOrderStatusHistory(orderStatusHistory);
                break;
            case 11:
                freeMemory(userList, orderHistoryList, orderStatusHistory);
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");

        }

    } while (choice != 11);


    return 0;
}
