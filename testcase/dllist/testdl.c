#include "dllist.h"
#include "testcase.h"

static dllist *dltemp;
static dllistnode *dlntemp;
static dllistnode *dlntemp2;
static dllistnode *dlntemp3;
static void *temp2;

BEGIN_TESTCASE(dllist)

CHECK_NEQU(dllist_alloc_with(&malloc), NULL);
UNALLOC_TEMP();

CHECK_NEQU(dllist_alloc(), NULL);
UNALLOC_TEMP();

dltemp = dllist_alloc();

TEST_CONDS_VOID(dllist_init_with(dltemp, &malloc));
COND(dltemp->head != NULL);
COND(dltemp->head->next == dltemp->head);
COND(dltemp->head->prev == dltemp->head);
COND(dltemp->size == 0);
COND(dltemp->alloc_callback == &malloc);
COND(dltemp->free_callback == NULL);
END_TEST_CONDS();

free(dltemp);

dltemp = dllist_alloc();

TEST_CONDS_VOID(dllist_init(dltemp));
COND(dltemp->head != NULL);
COND(dltemp->head->next == dltemp->head);
COND(dltemp->head->prev == dltemp->head);
COND(dltemp->size == 0);
COND(dltemp->alloc_callback == &malloc);
COND(dltemp->free_callback == NULL);
END_TEST_CONDS();

free(dltemp);

TEST_CONDS(dllist_create_with(&malloc));
dltemp = (dllist *)temp;
COND(dltemp->head != NULL);
COND(dltemp->head->next == dltemp->head);
COND(dltemp->head->prev == dltemp->head);
COND(dltemp->size == 0);
COND(dltemp->alloc_callback == &malloc);
COND(dltemp->free_callback == NULL);
END_TEST_CONDS();
UNALLOC_TEMP();

dltemp = NULL;

TEST_CONDS(dllist_create());
dltemp = (dllist *)temp;
COND(dltemp->head != NULL);
COND(dltemp->head->next == dltemp->head);
COND(dltemp->head->prev == dltemp->head);
COND(dltemp->size == 0);
COND(dltemp->alloc_callback == &malloc);
COND(dltemp->free_callback == NULL);
END_TEST_CONDS();
UNALLOC_TEMP();

dltemp = NULL;

dltemp = dllist_create();

TEST_CONDS_VOID(dllist_set_destructor(dltemp, &free));
COND(dltemp->free_callback == &free);
END_TEST_CONDS();

free(dltemp);

dlntemp = dllistnode_alloc();
dltemp = dllist_create();

TEST_CONDS_VOID(dllist_insert_node_after_head(dltemp, dlntemp));
COND(dltemp->head->next == dlntemp);
COND(dltemp->head->prev == dlntemp);
COND(dlntemp->next == dltemp->head);
COND(dlntemp->prev == dltemp->head);
COND(dltemp->size == 1);
END_TEST_CONDS();

dlntemp2 = dllistnode_alloc();

TEST_CONDS_VOID(dllist_insert_node_before_head(dltemp, dlntemp2));
COND(dltemp->head->next == dlntemp);
COND(dltemp->head->prev == dlntemp2);
COND(dltemp->head->next->next == dlntemp2);
COND(dltemp->head->next->next->next == dltemp->head);
COND(dltemp->head->prev->prev == dlntemp);
COND(dltemp->head->prev->prev->prev == dltemp->head);
COND(dlntemp2->next == dltemp->head);
COND(dlntemp2->prev == dlntemp);
COND(dltemp->size == 2);
END_TEST_CONDS();

TEST_CONDS(dllist_remove_first_node(dltemp));
dlntemp3 = (dllistnode *)temp;
COND(dlntemp == dlntemp3);
COND(dltemp->size == 1);
COND(dltemp->head->next == dlntemp2);
COND(dltemp->head->prev == dlntemp2);
END_TEST_CONDS();

TEST_CONDS(dllist_remove_last_node(dltemp));
dlntemp3 = (dllistnode *)temp;
COND(dlntemp2 == dlntemp3);
COND(dltemp->size == 0);
COND(dltemp->head->next == dltemp->head);
COND(dltemp->head->prev == dltemp->head);
END_TEST_CONDS();

dllist_insert_node_after_head(dltemp, dlntemp);

dltemp->head->next->elem = (void *)0xDEAD;
temp2 = dlntemp->elem; // should be the same as above

TEST_CONDS(dllist_remove_node(dltemp, dlntemp));
COND(temp == temp2);
COND(dltemp->head->next == dltemp->head);
COND(dltemp->head->prev == dltemp->head);
COND(dltemp->size == 0);
END_TEST_CONDS();

free(dltemp);
dltemp = dllist_create();

TEST_CONDS_VOID(dllist_push(dltemp, (void *)0xDEAD));
COND(dltemp->head->next->elem == (void *)0xDEAD);
COND(dltemp->head->prev->elem == (void *)0xDEAD);
COND(dltemp->head->next->next == dltemp->head);
COND(dltemp->head->prev->prev == dltemp->head);
COND(dltemp->size == 1);
END_TEST_CONDS();

TEST_CONDS_VOID(dllist_enqueue(dltemp, (void *)0xFED5));
COND(dltemp->head->next->elem == (void *)0xDEAD);
COND(dltemp->head->prev->elem == (void *)0xFED5);
COND(dltemp->head->next->next->elem == (void *)0xFED5);
COND(dltemp->head->prev->prev->elem == (void *)0xDEAD);
COND(dltemp->head->next->next->next == dltemp->head);
COND(dltemp->head->prev->prev->prev == dltemp->head);
COND(dltemp->size == 2);
END_TEST_CONDS();

TEST_CONDS(dllist_pop(dltemp));
COND(temp == (void *)0xDEAD);
COND(dltemp->head->next->elem == (void *)0xFED5);
COND(dltemp->head->prev->elem == (void *)0xFED5);
COND(dltemp->head->next->next == dltemp->head);
COND(dltemp->head->prev->prev == dltemp->head);
COND(dltemp->size == 1);
END_TEST_CONDS();

TEST_CONDS(dllist_dequeue(dltemp));
COND(temp == (void *)0xFED5);
COND(dltemp->head->next == dltemp->head);
COND(dltemp->head->prev == dltemp->head);
COND(dltemp->size == 0);
END_TEST_CONDS();

free(dltemp);
dltemp = dllist_create();
dllist_push(dltemp, (void *)1);
dllist_push(dltemp, (void *)2);

TEST_CONDS_VOID(dllist_empty(dltemp));
COND(dltemp->size == 0);
COND(dltemp->head->next == dltemp->head);
COND(dltemp->head->prev == dltemp->head);
END_TEST_CONDS();

free(dltemp);

END_OF_TESTCASE()
