#define TT_TRACE_MODULE_NAME    "list_entry_demo"
#define TT_TRACE_DEBUG          1

#include "ttlib.h"
#include "color.h" 

typedef struct __tt_demo_list_entry_t{
	tt_list_entry_t           entry;
	tt_size_t                 user_data;
}tt_demo_list_entry_t, *tt_demo_list_entry_ref_t;

static tt_void_t tt_demo_entry_copy_t(tt_pointer_t litem, tt_pointer_t ritem)
{
	tt_assert(litem && ritem);

	/// copy data
	((tt_demo_list_entry_ref_t)litem)->user_data = ((tt_demo_list_entry_ref_t)ritem)->user_data;
}

/// list
static tt_list_entry_head_t g_list;
static tt_demo_list_entry_t node[10] = {
	{ { 0, 0 }, 0 },
	{ { 0, 0 }, 1 },
	{ { 0, 0 }, 2 },
	{ { 0, 0 }, 3 },
	{ { 0, 0 }, 4 },
	{ { 0, 0 }, 5 },
	{ { 0, 0 }, 6 },
	{ { 0, 0 }, 7 },
	{ { 0, 0 }, 8 },
	{ { 0, 0 }, 9 },
};

tt_void_t tt_demo_list_entry_main(tt_void_t)
{
	// print title
	tt_print_title("demo list entry");

	tt_list_entry_init(&g_list, tt_demo_list_entry_t, entry, tt_demo_entry_copy_t);

	tt_size_t entry = 0;
	tt_demo_list_entry_ref_t node_ref = tt_null;
	tt_iterator_ref_t iterator = tt_list_entry_iterator(&g_list);
	
//	tt_size_t head = tt_iterator_heaad

	/// test single list entry interfaces
	tt_list_entry_insert_tail(&g_list, &node[2].entry);
	tt_list_entry_insert_tail(&g_list, &node[3].entry);
	tt_list_entry_insert_tail(&g_list, &node[4].entry);
	tt_list_entry_insert_head(&g_list, &node[1].entry);
	tt_list_entry_insert_head(&g_list, &node[0].entry);
	
	tt_trace_d("\n---------------");
	/// walk list
	for (entry = tt_iterator_head(iterator); entry != tt_iterator_tail(iterator); entry = tt_iterator_next(iterator, entry))
	{
		node_ref = (tt_demo_list_entry_ref_t)tt_iterator_item(iterator, entry);
		tt_trace_d("node entry, %X, node data, %d", node_ref, node_ref->user_data);
	}

	tt_list_entry_replace_head(&g_list, &node[5].entry);

	tt_trace_d("\n---------------");
	/// walk list
	for (entry = tt_iterator_head(iterator); entry != tt_iterator_tail(iterator); entry = tt_iterator_next(iterator, entry))
	{
		node_ref = (tt_demo_list_entry_ref_t)tt_iterator_item(iterator, entry);
		tt_trace_d("node entry, %X, node data, %d", node_ref, node_ref->user_data);
	}

	tt_list_entry_remove_head(&g_list);

	tt_trace_d("---------------");
	/// walk list
	for (entry = tt_iterator_head(iterator); entry != tt_iterator_tail(iterator); entry = tt_iterator_next(iterator, entry))
	{
		node_ref = (tt_demo_list_entry_ref_t)tt_iterator_item(iterator, entry);
		tt_trace_d("node entry, %X, node data, %d", node_ref, node_ref->user_data);
	}
	
	/// size
	tt_trace_d("size, %d", tt_list_entry_size(&g_list));

	/// head
	tt_list_entry_ref_t node_head = tt_list_entry_head(&g_list);
	tt_trace_d("node_head, %X", node_head);

	/// last
	tt_list_entry_ref_t node_last = tt_list_entry_last(&g_list);
	tt_trace_d("node_last, %X", node_last);


	tt_trace_d("---------------iterator");
	/// step
	tt_trace_d("step, %d", tt_iterator_step(iterator));

	/// mode
	tt_trace_d("mode, %d", tt_iterator_mode(iterator));

	/// size
	tt_trace_d("size, %d", tt_iterator_size(iterator));

	/// head
	tt_trace_d("head, %X", tt_iterator_head(iterator));

	/// last
	tt_trace_d("last, %X", tt_iterator_last(iterator));

	/// remove	
//	tt_iterator_remove(iterator, tt_iterator_last(iterator));

	/// walk list
	for (entry = tt_iterator_head(iterator); entry != tt_iterator_tail(iterator); entry = tt_iterator_next(iterator, entry))
	{
		node_ref = (tt_demo_list_entry_ref_t)tt_iterator_item(iterator, entry);
		tt_trace_d("node entry, %X, node data, %d", node_ref, node_ref->user_data);
	}

	/// test tt_for with single_list_entry
	tt_trace_d("---------------tt_for");
	tt_for_all(tt_demo_list_entry_ref_t, item0, iterator)
	{
		tt_trace_d("%d", (tt_demo_list_entry_ref_t)item0->user_data);
	}

	tt_trace_d("---------------tt_for_if");
	tt_for_all_if(tt_demo_list_entry_ref_t, item1, iterator, ((tt_demo_list_entry_ref_t)item1)->user_data >= 2)
	{
		tt_trace_d("%d", (tt_demo_list_entry_ref_t)item1->user_data);
	}
	tt_list_entry_exit(&g_list);
}

