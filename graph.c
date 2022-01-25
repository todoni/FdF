#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

// 그래프 생성
LinkedGraph* createLinkedGraph(int maxVertexCount)
{
    LinkedGraph* tmp = malloc(sizeof(LinkedGraph));
	if (!tmp)
	    return 0;
	tmp->maxVertexCount = maxVertexCount;
	tmp->currentVertexCount = 0;
	tmp->graphType = GRAPH_UNDIRECTED;
	LinkedList	**tmpList = calloc(maxVertexCount, sizeof(LinkedList*));
	if (!tmpList)
		return 0;
	for (int i = 0; i < maxVertexCount; i++)
	{
		tmpList[i] = calloc(1, sizeof(LinkedList));
		if (!tmpList[i])
		{
			while (--i > -1)
				free(tmpList[i]);
			free(tmpList);
			return 0;
		}
		tmpList[i]->vertexID = i;
	}
	tmp->ppAdjEdge = tmpList;
	int	*tmpVertex = calloc(maxVertexCount, sizeof(int));
	if (!tmpVertex)
		return 0;
	tmp->pVertex = tmpVertex;
	return tmp;
}

LinkedGraph* createLinkedDirectedGraph(int maxVertexCount)
{
    LinkedGraph* tmp = malloc(sizeof(LinkedGraph));
	if (!tmp)
	    return 0;
	tmp->maxVertexCount = maxVertexCount;
	tmp->currentVertexCount = 0;
	tmp->graphType = GRAPH_DIRECTED;
	LinkedList	**tmpList = calloc(maxVertexCount, sizeof(LinkedList*));
	if (!tmpList)
		return 0;
	for (int i = 0; i < maxVertexCount; i++)
	{
		tmpList[i] = calloc(1, sizeof(LinkedList));
		if (!tmpList[i])
		{
			while (--i > -1)
				free(tmpList[i]);
			free(tmpList);
			return 0;
		}
		tmpList[i]->vertexID = i;
	}
	tmp->ppAdjEdge = tmpList;
	int	*tmpVertex = calloc(maxVertexCount, sizeof(int));
	if (!tmpVertex)
		return 0;
	tmp->pVertex = tmpVertex;
	return tmp;
}

// 그래프 삭제
void deleteLinkedGraph(LinkedGraph* pGraph)
{
	if (!pGraph->currentVertexCount)
		return ;
	/* ppAdjEdge free */
	for (int i = 0; i < pGraph->maxVertexCount; i++)
	{
		while (pGraph->ppAdjEdge[i])
		{
		    LinkedList 	*tmp = pGraph->ppAdjEdge[i];
            pGraph->ppAdjEdge[i] = pGraph->ppAdjEdge[i]->next;
			free(tmp);
		}
	}
	free(pGraph->ppAdjEdge);
	pGraph->ppAdjEdge = NULL;
	free(pGraph->pVertex);
    pGraph->pVertex = NULL;
	free(pGraph);
	pGraph->maxVertexCount = pGraph->currentVertexCount = pGraph->currentEdgeCount = 0;
	printf("Graph deleted\n");
}

// 공백 그래프 여부 판단
int isEmptyLG(LinkedGraph* pGraph)
{
	if (pGraph->currentVertexCount)
		return FALSE;
	return TRUE;
}

// 노드 추가
int addVertexLG(LinkedGraph* pGraph, int vertexID)
{
	if (vertexID < 0 || vertexID >= pGraph->maxVertexCount)
		return FAIL;
	if (!pGraph->pVertex[vertexID])
   		pGraph->currentVertexCount++;
   	pGraph->pVertex[vertexID] = USED;
	return SUCCESS;
}

// 간선 추가
int addEdgeLG(LinkedGraph* pGraph, int fromVertexID, int toVertexID)
{
    if (!checkVertexValid(pGraph, fromVertexID) || !checkVertexValid(pGraph, toVertexID))
		return FAIL;
    // if ((fromVertexID < 0 || fromVertexID >= pGraph->maxVertexCount) || (toVertexID < 0 || toVertexID >= pGraph->maxVertexCount))
	// 	return FAIL;
	if (fromVertexID == toVertexID)
		return FAIL;
    if (findGraphNodePosition(pGraph->ppAdjEdge[fromVertexID], toVertexID))
        return (FAIL);

	LinkedList  *tmp = calloc(1, sizeof(LinkedList));
	if (!tmp)	return FAIL;
	tmp->vertexID = toVertexID;
	LinkedList	*ptr = pGraph->ppAdjEdge[fromVertexID];
	while (ptr->next)
		ptr = ptr->next;
    ptr->next = tmp;
	if (pGraph->graphType == GRAPH_UNDIRECTED)
	{
		tmp = calloc(1, sizeof(LinkedList));
		if (!tmp)	return FAIL;
		tmp->vertexID = fromVertexID;
		ptr = pGraph->ppAdjEdge[toVertexID];
		while (ptr->next)
			ptr = ptr->next;
    	ptr->next = tmp;
	}
    pGraph->currentEdgeCount++;
	return SUCCESS;
}

int addEdgewithWeightLG(LinkedGraph* pGraph, int fromVertexID, int toVertexID, int weight)
{
    if (!checkVertexValid(pGraph, fromVertexID) || !checkVertexValid(pGraph, toVertexID))
		return FAIL;
    // if ((fromVertexID < 0 || fromVertexID >= pGraph->maxVertexCount) || (toVertexID < 0 || toVertexID >= pGraph->maxVertexCount))
	// 	return FAIL;
	if (fromVertexID == toVertexID)
		return FAIL;
	if (findGraphNodePosition(pGraph->ppAdjEdge[fromVertexID], toVertexID))
		return FAIL;

	LinkedList  *tmp = calloc(1, sizeof(LinkedList));
    if (!tmp)
        return (FAIL);
	tmp->vertexID = toVertexID;
	tmp->weight = weight;
	LinkedList	*ptr = pGraph->ppAdjEdge[fromVertexID];
	while (ptr->next)
		ptr = ptr->next;
    ptr->next = tmp;
	if (pGraph->graphType == GRAPH_UNDIRECTED)
	{
		tmp = calloc(1, sizeof(LinkedList));
        if (!tmp)
        	return (FAIL);
		tmp->vertexID = fromVertexID;
		tmp->weight = weight;
		ptr = pGraph->ppAdjEdge[toVertexID];
		while (ptr->next)
			ptr = ptr->next;
    	ptr->next = tmp;
	}
	pGraph->currentEdgeCount++;
	return SUCCESS;
}

// 노드의 유효성 점검.
int checkVertexValid(LinkedGraph* pGraph, int vertexID)
{
	if (pGraph->pVertex[vertexID])
        return (TRUE);
    return (FALSE);
}

// 노드 제거
int removeVertexLG(LinkedGraph* pGraph, int vertexID)
{
	if (vertexID < 0 || vertexID >= pGraph->maxVertexCount)
		return FAIL;
	if (pGraph->pVertex[vertexID])
		pGraph->currentVertexCount--;
	pGraph->pVertex[vertexID] = NOT_USED;
	LinkedList	*ptr = pGraph->ppAdjEdge[vertexID];
	while (ptr)
	{
		LinkedList	*del = ptr;
		ptr = ptr->next;
		free(del);
	}
	return SUCCESS;
}

// 간선 제거
int removeEdgeLG(LinkedGraph* pGraph, int fromVertexID, int toVertexID)
{
	if (!checkVertexValid(pGraph, fromVertexID) || !checkVertexValid(pGraph, toVertexID))
		return FAIL;
	// if ((fromVertexID < 0 || fromVertexID >= pGraph->maxVertexCount) || (toVertexID < 0 || toVertexID >= pGraph->maxVertexCount))
	// 	return FAIL;
	LinkedList	*tmp = pGraph->ppAdjEdge[fromVertexID];
	LinkedList	*before;
	while (tmp->next)
	{
		if (tmp->vertexID == toVertexID)
			break ;
		before = tmp;
		tmp = tmp->next;
	}
	if (!tmp)	return FAIL;
	before->next = tmp->next;
	free(tmp);
    if (pGraph->graphType == GRAPH_UNDIRECTED)
	{
		tmp = pGraph->ppAdjEdge[toVertexID];
		while (tmp)
		{
			if (tmp->vertexID == fromVertexID)
				break ;
			before = tmp;
			tmp = tmp->next;
		}
		if (!tmp)	return FAIL;
		before->next = tmp->next;
		free(tmp);
	}
	pGraph->currentEdgeCount--;
	return SUCCESS;
}

void deleteGraphNode(LinkedList* pList, int delVertexID)
{
	LinkedList *tmp = pList;
    LinkedList *before;

    while (tmp)
    {
        if (tmp->vertexID == delVertexID)
            break;
        before = tmp;
        tmp = tmp->next;
    }
    if (!tmp)
        return ;
    before->next = tmp->next;
    free(tmp);
}

int findGraphNodePosition(LinkedList* pList, int vertexID)
{
	LinkedList *tmp = pList->next;

    while (tmp)
    {
        if (tmp->vertexID == vertexID)
            break;
        tmp = tmp->next;
    }
    if (!tmp)
        return (FAIL);
    return (SUCCESS);
}


// 간선 개수 반환
int getEdgeCountLG(LinkedGraph* pGraph)
{
	return pGraph->currentEdgeCount;
}

// 노드 개수 반환
int getVertexCountLG(LinkedGraph* pGraph)
{
    return (pGraph->currentVertexCount);
}

// 최대 노드 개수 반환
int getMaxVertexCountLG(LinkedGraph* pGraph)
{
	return pGraph->maxVertexCount;
}

// 그래프 종류 반환.
int getGraphTypeLG(LinkedGraph* pGraph)
{
	return pGraph->graphType;
}

// 그래프 정보 출력
void displayLinkedGraph(LinkedGraph* pGraph)
{
	if (!pGraph->currentVertexCount)
	{
        printf("Graph is Empty.\n");
        return ;
    }
	printf("\n< Graph > - ");
	if (pGraph->graphType == GRAPH_DIRECTED)
		printf("directed\n");
	else
		printf("undirected\n");
	/* ppAdjEdge print */
	for (int i = 0; i < pGraph->maxVertexCount; i++)
    {
        LinkedList *tmp = pGraph->ppAdjEdge[i];
		if (checkVertexValid(pGraph, tmp->vertexID))
			printf("\e[32m(O)");
		else	printf("\e[31m(X)");
        while (tmp)
        {
            printf("%d\e[0m", tmp->vertexID);
            tmp = tmp->next;
			if (tmp) printf(" -> ");
        }
        printf("\n");
    }
	/* pVertex print */
	printf("Vertex: ");
	for (int i = 0; i < pGraph->maxVertexCount; i++)
	{
		if (pGraph->pVertex[i])
			printf("\e[32m");
		else	printf("\e[31m");
		printf("%d\e[0m\t", pGraph->pVertex[i]);
	}
	printf("\n\n");
}

