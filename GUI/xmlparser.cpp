#include "xmlparser.h"

void InitStack(Stack *S, int initialSize)
{
    S->MAX = initialSize;
    S->top = 0;
    S->A = (char*)calloc(S->MAX, sizeof(char));
    if (S->A == NULL)
    {
        printf("Allocation failed");
        exit(0);
    }
}

void FreeStack(Stack *S)
{
    free(S->A);
    S->A = NULL;
}

void Push(Stack *S, char value)
{
    if (S->top < S->MAX)
    {
        S->A[S->top] = value;
        S->top++;
    }
    else
    {
        printf("Stack overflow");
    }
}

void Pop(Stack *S)
{
    if (S->top == 0)
    {
        printf("Empty Stack");
    }
    else
    {
        S->top--;
        S->A[S->top] = 0;
    }
}

bool IsStackEmpty(Stack *S)
{
    if (S->top == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

char stackTop(Stack *S)
{
    if (S->top > 0)
    {
        return S->A[S->top - 1];
    }
    else
    {
        printf("Empty Stack");
        return -1;
    }
}

bool checkBalance(const char *source, int n)
{
    int flag = 0;
    int opening = 0;
    int closing = 0;
    Stack s;
    InitStack(&s, 10);
    for (int i = 0; i < n; i++)
    {
        if (source[i] == '<')
        {
            if (source[i + 1] == '/')
            {
                closing++;
            }
            else
            {
                opening++;
            }
            Push(&s, '<');
        }
        else if (source[i] == '>')
        {
            if (IsStackEmpty(&s))
            {
                flag++;
            }
            else if (stackTop(&s) == '<')
            {
                Pop(&s);
            }
        }
    }
    if (flag != 0 || (closing - opening) != 0)
    {
        return false;
    }
    else if (IsStackEmpty(&s) && (closing - opening) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void XMLNodeList_init(XMLNodeList *list)
{
    list->MAX = 1;
    list->size = 0;
    // list of pointers to xml nodes
    list->data = (XMLNode **)malloc(sizeof(XMLNode *) * list->MAX);
}

void XMLNodeList_add(XMLNodeList *list, XMLNode *node)
{
    // When dynamic array exceeds maximum size, double capacity
    while (list->size >= list->MAX)
    {
        list->MAX *= 2;
        // list of pointers to xml nodes
        list->data = (XMLNode **)realloc(list->data, sizeof(XMLNode *) * list->MAX);
    }
    list->data[list->size++] = node;
}

XMLNode *XMLNodeList_at(XMLNodeList *list, int index)
{
    if (index > list->size)
    {
        // Assert
        printf("Wrong index for node list");
        exit(0);
    }
    else
    {
        // return required node from list of nodes
        return list->data[index];
    }
}

void XMLNodeList_free(XMLNodeList *list)
{
    free(list);
}

// Create new nodes
XMLNode *XMLNode_new(XMLNode *parent)
{
    // XML Node pointer to a heap allocated memory
    XMLNode *node = (XMLNode *)malloc(sizeof(XMLNode));
    node->parent = parent; // set the parent node
    // Initialization
    //<tag>Inner_text</tag>
    node->tag = NULL;
    node->inner_text = NULL;
    node->inner_text_size = 0;
    node->flag = 0;
    // initialize list of children
    XMLNodeList_init(&node->children);
    if (parent) // not equal to null
    {
        XMLNodeList_add(&parent->children, node);
    }
    return node;
}

void XMLNode_free(XMLNode *node)
{
    if (node->tag)
    {
        free(node->tag); // if not null free
    }
    if (node->inner_text)
    {
        free(node->inner_text); // if not null free
    }
    free(node);
}

XMLNode *XMLNode_child(XMLNode *parent, int index)
{
    if (index > parent->children.size)
    {
        // assert
        printf("wrong indexing for child nodes\n");
        exit(0);
    }
    else
    {
        return parent->children.data[index];
    }
}

XMLNodeList *XMLNode_children(XMLNode *parent, const char *tag)
{
    XMLNodeList *list = (XMLNodeList *)malloc(sizeof(XMLNodeList));
    XMLNodeList_init(list);
    for (int i = 0; i < parent->children.size; i++)
    {
        XMLNode *child = parent->children.data[i];
        if (!strcmp(child->tag, tag))
        {
            XMLNodeList_add(list, child);
        }
    }
    return list;
}

// Load xml to memory
bool XMLDocument_load(XMLDocument *doc, const char *path)
{
    // Create a new file pointer, "r" open it for reading and return NULL if it doesnt exist
    FILE *file = fopen(path, "r");
    // Check if file loads properly
    if (!file)
    {
        // output to stderr to seperate error messages from regular output
        fprintf(stderr, "Couldnt load file");
        return false;
    }
    int newlines = 0;
    char ch;
    // count number of new lines
    //  a new line is represented by two bytes in a text file,
    //  but when reading it only yields a single '\n' character.
    while (file)
    {
        ch = fgetc(file);
        if (ch == EOF)
            break;
        if (ch == '\n')
            newlines++;
    }
    // move file pointer to end of file to get size
    fseek(file, 0, SEEK_END);
    // return current value of file pointer to get size
    int size = ftell(file);
    // Move file pointer back to the beginning
    fseek(file, 0, SEEK_SET);
    // Create new Character heap allocated array to hold the file data
    // extra byte for null termination
    char *source = (char *)malloc(sizeof(char) * size + 1);
    // read "size" bytes from file
    fread(source, 1, size, file);
    fclose(file);
    /*-----------------------------------------------------*/

    // null terminate
    source[size - newlines] = '\0';

    // check consistency
    if (checkBalance(source, size))
    {
        printf("the xml is balanced\n");
    }
    else
    {
        printf("The XML is not balanced\n");
        // Abdo add return
    }

    // Create new root node (no parent)
    doc->root = XMLNode_new(NULL);
    // char array to extract information (Vector in c++)
    char buffer[2560];
    // index to control the buffer array
    int buffer_index = 0;
    // index of source array
    int i = 0;

    // first node to work with is the doc root node
    XMLNode *curr_node = doc->root;

    // Parsing loop (Seperate function)
    // as long as we dont reach end of source array
    while (source[i] != '\0')
    {
        // if we find an open tag
        if (source[i] == '<')
        {
            // Reset the buffer
            buffer[buffer_index] = '\0';
            // Inner Text
            // If buffer index is not zero then there is inner text
            if (buffer_index > 0)
            {
                // Append extracted inner text to our buffer and make note of size
                curr_node->inner_text_size += buffer_index;
                curr_node->inner_text = strdup(buffer);
                // reset buffer index
                buffer_index = 0;
            }

            // End of node
            // if we find a '/' after '<' we are at the end of node
            if (source[i + 1] == '/')
            {
                // move two positions <,/
                i += 2;
                // while we havent reached end tag append to buffer
                while (source[i] != '>')
                {
                    buffer[buffer_index++] = source[i++];
                }
                // once we reach end tag add null termination to denote the extracted string
                buffer[buffer_index] = '\0';
                // If current Node is NULL it means only end tag is there
                if (!curr_node->tag)
                {
                    fprintf(stderr, "Missing start tag\n");
                    return false;
                }
                // Check if tags match
                // strcmp returns 0 if they match
                if (strcmp(curr_node->tag, buffer))
                {
                    fprintf(stderr, "Mismatched tags (%s != %s)\n", curr_node->tag, buffer);
                    char choice;
                    printf("Do you want to correct the error and match with %s tag: enter 'y' or 'n' ", curr_node->tag);
                    scanf(" %c", &choice);
                    // Correct mismatched tags
                    if (choice == 'y')
                    {
                       // *buffer = strdup(curr_node->tag);
                    }
                    else
                    {
                        return false;
                    }
                }
                // If they match we are done move to parent
                curr_node = curr_node->parent;
                buffer_index = 0;
                i++;
                continue;
            }
            // Comments and special nodes <!--->
            if (source[i + 1] == '!')
            {
                while (source[i] != '>')
                {
                    if (buffer[buffer_index - 1] == ' ')
                    {
                        // dont pickup space
                        buffer_index--;
                        continue;
                    }
                    buffer[buffer_index++] = source[i++];
                }
                buffer[buffer_index] = '\0';
                // Comments
                if (buffer[buffer_index - 1] == '-' && buffer[buffer_index - 2] == '-')
                {
                    continue;
                }
            }

            // Create a new node with the current node being the parent

            curr_node = XMLNode_new(curr_node);

            // Start Tag
            // increment where we are in the source
            i++;
            // While we havent reached the end tag
            while (source[i] != '>')
            {
                // add the data to the buffer
                buffer[buffer_index++] = source[i++];
                // <tag  >
                // if we find a space and we still dont have a tag
                // skip space and set tag
                if (source[i] == ' ' && !curr_node->tag)
                {
                    // denote the end of extracted string
                    buffer[buffer_index] = '\0';
                    // set the tag
                    curr_node->tag = strdup(buffer);
                    // reset buffer index
                    buffer_index = 0;
                    i++;
                    continue;
                }
                // if last character is space
                // ignore other spaces
                if (buffer[buffer_index - 1] == ' ')
                {
                    // dont pickup space
                    buffer_index--;
                    continue;
                }
            }
            // set tag name if it doesnt exist
            buffer[buffer_index] = '\0';
            if (!curr_node->tag)
            {
                curr_node->tag = strdup(buffer);
            }
            // Reset
            buffer_index = 0; // reset
            i++;
            continue;
        }
        else
        // if not inside tag
        // keep appending inner text to the buffer
        {
            if (source[i] == '\n')
            {
                i++;
                // found white space after newline
                while (source[i] == ' ')
                {
                    // ignore white space dont add to buffer
                    i++;
                }
            }
            else
            {
                buffer[buffer_index++] = source[i++];
            }
        }
    }
    // if current node is NULL and the index is not zero then there is an outside text
    if (buffer_index > 0 && curr_node->tag == NULL)
    {
        fprintf(stderr, "text outside of document\n");
        return false;
    }
    else if (curr_node->tag)
    {
        fprintf(stderr, "missing end tag\n");
        curr_node = curr_node->parent;
        // return false;
    }
    return true;
}
bool are_same_children(XMLNode *parent)
{
    int mark = 0;
    for (int i = 0; i < parent->children.size - 1; i++)
    {
        if (strcmp(parent->children.data[i]->tag, parent->children.data[i + 1]->tag))
        {
            return false;
        }
        else
        {
            if (mark == 0)
            {
                parent->children.data[i]->flag = 2;
                if (i == (int)parent->children.size - 2)
                {
                    parent->children.data[i + 1]->flag = 4;
                }
                else
                {
                    parent->children.data[i + 1]->flag = 1;
                }
                mark++;
            }
            else
            {
                parent->children.data[i]->flag = 1;
                if (i == (int)parent->children.size - 2)
                {
                    parent->children.data[i + 1]->flag = 4;
                }
                else
                {
                    parent->children.data[i + 1]->flag = 1;
                }
            }
        }
    }
    return true;
}

static void JSON(FILE *file, XMLNode *node, int indent, int times)
{
    for (int i = 0; i < node->children.size; i++)
    {
        XMLNode *child = node->children.data[i];
        if (times > 0)
        {
            fprintf(file, "%0*s", indent * times, " ");
        }
        if (child->children.size == 0)
        {
            if (child->flag == 2)
            {
                fprintf(file, "\"%s\":[\n", child->tag);
                fprintf(file, "%0*s", indent * times, " ");
                fprintf(file, "{\n");
            }
            if (child->inner_text_size > 200) // if inner text size is large make new line
            {
                if (child->flag != 1 && child->flag != 4)
                {
                    fprintf(file, "\"%s\": ", child->tag);
                }
                fprintf(file, "\n%0*s\"%s\",\n%0*s\n", (indent * (times + 1)), " ", child->inner_text, (indent * (times)), " ");
            }
            else // same line if inner text small
            {
                if (child->flag != 1 && child->flag != 4 && child->flag != 2)
                {
                    fprintf(file, "\"%s\": ", child->tag);
                }
                fprintf(file, "\"%s\",\n", child->inner_text);
            }
            if (child->flag == 4)
            {
                fprintf(file, "]\n");
            }
        }
        else
        {
            if (are_same_children(child) && child->children.size >= 1)
            {
                child->flag = 3;
                fprintf(file, "\"%s\" :{\n", child->tag);
            }
            else
            {
                // if (child->children.size == 1)
                // {
                //     fprintf(file, "\"%s\" :{\n", child->tag);
                // }
                if (child->flag == 2)
                {
                    fprintf(file, "\"%s\":[\n", child->tag);
                    fprintf(file, "%0*s", indent * times, " ");
                    fprintf(file, "{\n");
                }
                else if (child->flag == 3)
                {
                }
                else if (child->flag != 1 && child->flag != 4)
                {
                    fprintf(file, "\"%s\": ", child->tag);
                }
            }
            fprintf(file, "\n");
            JSON(file, child, indent, times + 1);
            if (times > 0)
            {
                fprintf(file, "%0*s", indent * times, " ");
            }
            fprintf(file, "}\n");
        }
    }
}

static void Format(FILE *file, XMLNode *node, int indent, int times)
{
    for (int i = 0; i < node->children.size; i++)
    {
        XMLNode *child = node->children.data[i];
        if (times > 0)
        {
            fprintf(file, "%0*s", indent * times, " ");
        }
        fprintf(file, "<%s", child->tag);
        // no inner text
        if (child->children.size == 0 && !child->inner_text)
        {
            fprintf(file, " />\n");
        }
        else
        {
            fprintf(file, ">");
            if (child->children.size == 0)
            {
                if (child->inner_text_size > 200) // if inner text size is large make new line
                {
                    fprintf(file, "\n%0*s%s\n%0*s</%s>\n", (indent * (times + 1)), " ", child->inner_text, (indent * (times)), " ", child->tag);
                }
                else // same line if inner text small
                {
                    fprintf(file, "%s</%s>\n", child->inner_text, child->tag);
                }
            }
            else
            {
                fprintf(file, "\n");
                Format(file, child, indent, times + 1);
                if (times > 0)
                {
                    fprintf(file, "%0*s", indent * times, " ");
                }
                fprintf(file, "</%s>\n", child->tag);
            }
        }
    }
}

static void Minify(FILE *file, XMLNode *node)
{
    for (int i = 0; i < node->children.size; i++)
    {
        XMLNode *child = node->children.data[i];
        fprintf(file, "<%s", child->tag);
        // no inner text
        if (child->children.size == 0 && !child->inner_text)
        {
            fprintf(file, " />");
        }
        else
        {
            fprintf(file, ">");
            if (child->children.size == 0)
            {
                fprintf(file, "%s</%s>", child->inner_text, child->tag);
            }
            else
            {
                Minify(file, child);
                fprintf(file, "</%s>", child->tag);
            }
        }
    }
}

bool XMLDocument_write(XMLDocument *doc, const char *path, const char choice)
{
    FILE *file = fopen(path, "w");
    if (!file)
    {
        fprintf(stderr, "Cannot open file to write");
        return false;
    }
    if (choice == 'f')
    {
        Format(file, doc->root, 3, 0); // identation = 3 spaces (can be changed)
    }
    else if (choice == 'm')
    {
        Minify(file, doc->root);
    }
    else if (choice == 'j')
    {
        fprintf(file, "{\n");
        JSON(file, doc->root, 3, 1);
        fprintf(file, "}");
    }
    fclose(file);
    return true;
}

void XMLDocument_free(XMLDocument *doc)
{
    XMLNode_free(doc->root); // free root node
}

void print_ID(XMLNode *user)
{
    XMLNode *id = XMLNode_child(user, 0);
    printf("%s: %s\n", id->tag, id->inner_text);
}

void print_Name(XMLNode *user)
{
    XMLNode *name = XMLNode_child(user, 1);
    printf("%s: %s\n", name->tag, name->inner_text);
}

void print_Posts(XMLNode *user)
{
    XMLNode *post = XMLNode_child(user, 2);
    XMLNodeList *posts = XMLNode_children(post, "post");
    for (int i = 0; i < posts->size; i++)
    {
        XMLNode *field = XMLNodeList_at(posts, i);
        printf("--------------------\n");
        printf("%s : %d\n", field->tag, i + 1);
        printf("--------------------\n");
        XMLNode *body = XMLNode_child(field, 0);
        printf("%s: %s\n", body->tag, body->inner_text);
        XMLNode *topics = XMLNode_child(field, 1);
        XMLNodeList *topic = XMLNode_children(topics, "topic");
        for (int j = 0; j < topic->size; j++)
        {
            XMLNode *mawdoo = XMLNodeList_at(topic, j);
            printf("%s: %s\n", mawdoo->tag, mawdoo->inner_text);
        }
    }
}

void print_Followers(XMLNode *user)
{
    XMLNode *follower = XMLNode_child(user, 3);
    XMLNodeList *followers = XMLNode_children(follower, "follower");
    for (int i = 0; i < followers->size; i++)
    {
        XMLNode *field = XMLNodeList_at(followers, i);
        printf("%s: %d\n", field->tag, i + 1);
        XMLNode *id = XMLNode_child(field, 0);
        printf("%s: %s\n", id->tag, id->inner_text);
    }
}

char *get_Follower_id(XMLNode *user, int index)
{
    XMLNode *follower = XMLNode_child(user, 3);
    XMLNodeList *followers = XMLNode_children(follower, "follower");
    if (followers->size >= index)
    {
        XMLNode *field = XMLNodeList_at(followers, index);
        XMLNode *id = XMLNode_child(field, 0);
        return id->inner_text;
    }
    else
    {
        return "fail";
    }
}

XMLNode *getUser_ID(XMLNode *users, const char *ID)
{
    for (int i = 0; i < users->children.size; i++)
    {
        XMLNode *user = XMLNode_child(users, i);
        XMLNode *id = XMLNode_child(user, 0);
        if (!strcmp(id->inner_text, ID))
        {
            return users->children.data[i];
        }
    }
}

XMLNode *getUser_Name(XMLNode *users, const char *Name)
{
    for (int i = 0; i < users->children.size; i++)
    {
        XMLNode *user = XMLNode_child(users, i);
        XMLNode *name = XMLNode_child(user, 1);
        if (!strcmp(name->inner_text, Name))
        {
            return users->children.data[i];
        }
    }
}
