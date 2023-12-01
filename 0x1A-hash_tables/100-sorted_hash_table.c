#include "hash_tables.h"

shash_table_t *shash_table_create(unsigned long int size);
int shash_table_set(shash_table_t *ht, const char *key, const char *value);
char *shash_table_get(const shash_table_t *ht, const char *key);
void shash_table_print(const shash_table_t *ht);
void shash_table_print_rev(const shash_table_t *ht);
void shash_table_delete(shash_table_t *ht);

/**
 * shash_table_create - Function creates hash table
 * @size: The size
 *
 * Return: NULL if an error occurs, otherwise a pointer
 */
shash_table_t *shash_table_create(unsigned long int size)
{
	shash_table_t *ht;
	unsigned long int v;

	ht = malloc(sizeof(shash_table_t));
	if (ht == NULL)
		return (NULL);

	ht->size = size;
	ht->array = malloc(sizeof(shash_node_t *) * size);
	if (ht->array == NULL)
		return (NULL);
	for (v = 0; v < size; v++)
		ht->array[v] = NULL;
	ht->shead = NULL;
	ht->stail = NULL;

	return (ht);
}

/**
 * shash_table_set - Function adds an element to hash table
 * @ht: Pointer to hash table
 * @key: the key
 * @value: the value
 *
 * Return: 0 if failed, -1 otherwise
 */
int shash_table_set(shash_table_t *ht, const char *key, const char *value)
{
	shash_node_t *new, *k;
	char *duplicate;
	unsigned long int j;

	if (ht == NULL || key == NULL || *key == '\0' || value == NULL)
		return (0);

	duplicate = strdup(value);
	if (duplicate == NULL)
		return (0);

	j = key_index((const unsigned char *)key, ht->size);
	k = ht->shead;
	while (k)
	{
		if (strcmp(k->key, key) == 0)
		{
			free(k->value);
			k->value = duplicate;
			return (1);
		}
		k = k->snext;
	}

	new = malloc(sizeof(shash_node_t));
	if (new == NULL)
	{
		free(duplicate);
		return (0);
	}
	new->key = strdup(key);
	if (new->key == NULL)
	{
		free(duplicate);
		free(new);
		return (0);
	}
	new->value = duplicate;
	new->next = ht->array[j];
	ht->array[j] = new;

	if (ht->shead == NULL)
	{
		new->sprev = NULL;
		new->snext = NULL;
		ht->shead = new;
		ht->stail = new;
	}
	else if (strcmp(ht->shead->key, key) > 0)
	{
		new->sprev = NULL;
		new->snext = ht->shead;
		ht->shead->sprev = new;
		ht->shead = new;
	}
	else
	{
		k = ht->shead;
		while (k->snext != NULL && strcmp(k->snext->key, key) < 0)
			k = k->snext;
		new->sprev = k;
		new->snext = k->snext;
		if (k->snext == NULL)
			ht->stail = new;
		else
			k->snext->sprev = new;
		k->snext = new;
	}

	return (1);
}

/**
 * shash_table_get - Function retrieves the val in hash table
 * @ht: Pointer to hash table
 * @key: the key
 *
 * Return: NULL if absent,Otherwise the val linked to the key
 */
char *shash_table_get(const shash_table_t *ht, const char *key)
{
	shash_node_t *fac;
	unsigned long int j;

	if (ht == NULL || key == NULL || *key == '\0')
		return (NULL);

	j = key_index((const unsigned char *)key, ht->size);
	if (j >= ht->size)
		return (NULL);

	fac = ht->shead;
	while (fac != NULL && strcmp(fac->key, key) != 0)
		fac = fac->snext;

	return ((fac == NULL) ? NULL : fac->value);
}

/**
 * shash_table_print - Function prints hash table
 * @ht: Pointer to hash table
 */
void shash_table_print(const shash_table_t *ht)
{
	shash_node_t *fac;

	if (ht == NULL)
		return;

	fac = ht->shead;
	printf("{");
	while (fac != NULL)
	{
		printf("'%s': '%s'", fac->key, fac->value);
		fac = fac->snext;
		if (fac != NULL)
			printf(", ");
	}
	printf("}\n");
}

/**
 * shash_table_print_rev - Function prints hash table in reverse
 * @ht: Pointer to hash table
 */
void shash_table_print_rev(const shash_table_t *ht)
{
	shash_node_t *fac;

	if (ht == NULL)
		return;

	fac = ht->stail;
	printf("{");
	while (fac != NULL)
	{
		printf("'%s': '%s'", fac->key, fac->value);
		fac = fac->sprev;
		if (fac != NULL)
			printf(", ");
	}
	printf("}\n");
}

/**
 * shash_table_delete - Function deletes hash table
 * @ht: Pointer to hash table
 */
void shash_table_delete(shash_table_t *ht)
{
	shash_table_t *head = ht;
	shash_node_t *fac;
	shash_node_t *h;

	if (ht == NULL)
		return;

	fac = ht->shead;
	while (fac)
	{
		h = fac->snext;
		free(fac->key);
		free(fac->value);
		free(fac);
		fac = h;
	}

	free(head->array);
	free(head);
}
