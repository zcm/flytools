#include "llist/sllist.h"
#include "testcase/testcase.h"

static sllist *sltemp;
static sllistnode *slntemp;
static sllistnode *slntemp2;
static void *temp2;

BEGIN_TESTCASE(sllist)

CHECK_NEQU(sllist_alloc_with(&malloc), NULL);
UNALLOC_TEMP();

CHECK_NEQU(sllist_alloc(), NULL);
UNALLOC_TEMP();

sltemp = sllist_alloc();

TEST_CONDS_VOID(sllist_init_with(sltemp, &malloc));
COND(sltemp->head != NULL);
COND(sltemp->head->next == sltemp->head);
COND(sltemp->size == 0);
COND(sltemp->alloc_callback == &malloc);
COND(sltemp->free_callback == NULL);
END_TEST_CONDS();

free(sltemp);

sltemp = sllist_alloc();

TEST_CONDS_VOID(sllist_init(sltemp));
COND(sltemp->head != NULL);
COND(sltemp->head->next == sltemp->head);
COND(sltemp->size == 0);
COND(sltemp->alloc_callback == &malloc);
COND(sltemp->free_callback == NULL);
END_TEST_CONDS();

free(sltemp);

TEST_CONDS(sllist_create_with(&malloc));
sltemp = (sllist *)temp;
COND(sltemp->head != NULL);
COND(sltemp->head->next == sltemp->head);
COND(sltemp->size == 0);
COND(sltemp->alloc_callback == &malloc);
COND(sltemp->free_callback == NULL);
END_TEST_CONDS();
UNALLOC_TEMP();

sltemp = NULL;

TEST_CONDS(sllist_create());
sltemp = (sllist *)temp;
COND(sltemp->head != NULL);
COND(sltemp->head->next == sltemp->head);
COND(sltemp->size == 0);
COND(sltemp->alloc_callback == &malloc);
COND(sltemp->free_callback == NULL);
END_TEST_CONDS();
UNALLOC_TEMP();

sltemp = NULL;

sltemp = sllist_create();

TEST_CONDS_VOID(sllist_set_destructor(sltemp, &free));
COND(sltemp->free_callback == &free);
END_TEST_CONDS();

free(sltemp);

slntemp = sllistnode_alloc();
sltemp = sllist_create();

TEST_CONDS_VOID(sllist_insert_node_after_head(sltemp, slntemp));
COND(sltemp->head->next == slntemp);
COND(slntemp->next == sltemp->head);
COND(sltemp->size == 1);
END_TEST_CONDS();

TEST_CONDS(sllist_remove_first_node(sltemp));
slntemp2 = (sllistnode *)temp;
COND(slntemp == slntemp2);
COND(sltemp->size == 0);
COND(sltemp->head->next == sltemp->head);
END_TEST_CONDS();

sllist_insert_node_after_head(sltemp, slntemp);

TEST_CONDS(sllist_get_prev_node(sltemp, slntemp));
slntemp2 = (sllistnode *)temp;
COND(slntemp2 == sltemp->head);
COND(sltemp->size == 1);
END_TEST_CONDS();

sltemp->head->next->elem = (void *)0xDEAD;
temp2 = slntemp->elem; // should be the same as above

TEST_CONDS(sllist_remove_node(sltemp, slntemp));
COND(temp == temp2);
COND(sltemp->head->next == sltemp->head);
COND(sltemp->size == 0);
END_TEST_CONDS();

free(sltemp);
sltemp = sllist_create();

TEST_CONDS_VOID(sllist_push(sltemp, (void *)0xDEAD));
COND(sltemp->head->next->elem == (void *)0xDEAD);
COND(sltemp->head->next->next == sltemp->head);
COND(sltemp->size == 1);
END_TEST_CONDS();

TEST_CONDS(sllist_pop(sltemp));
COND(temp == (void *)0xDEAD);
COND(sltemp->head->next == sltemp->head);
COND(sltemp->size == 0);
END_TEST_CONDS();

free(sltemp);
sltemp = sllist_create();
sllist_push(sltemp, (void *)1);
sllist_push(sltemp, (void *)2);

TEST_CONDS_VOID(sllist_empty(sltemp));
COND(sltemp->size == 0);
COND(sltemp->head->next == sltemp->head);
END_TEST_CONDS();

free(sltemp);

END_OF_TESTCASE()
