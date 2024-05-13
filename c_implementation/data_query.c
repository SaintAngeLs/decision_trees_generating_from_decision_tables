#include "data_query.h"
#include "string_tree.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

static int convertToInt(DataQueryKey* dst, DataQueryKey data, size_t lvl);
static int convertToReal(DataQueryKey* dst, DataQueryKey data, size_t lvl);

static int create_union(DataQueryKey* dst, DataQueryKey data, size_t lvl);
static int calculate_avg(DataQueryKey* dst, DataQueryKey data, size_t lvl);
static int calculate_sum(DataQueryKey* dst, DataQueryKey data, size_t lvl);
static int calculate_multiplication(DataQueryKey* dst, DataQueryKey data, size_t lvl);
static int calculate_min(DataQueryKey* dst, DataQueryKey data, size_t lvl);
static int calculate_max(DataQueryKey* dst, DataQueryKey data, size_t lvl);
static int get_count(DataQueryKey* dst, DataQueryKey data, size_t lvl);

static int convertToStr(DataQueryKey* dst, DataQueryKey data, size_t lvl);

void freeKey(DataQueryKey* data) {
  if (!data) return;
  if (data->type == DQString) {
    if (data->key.data.str.n)
      free(data->key.data.str.ptr);
  }
  else if (data->type == DQList) {
    DataQueryKey* p = data->key.data.list.root;
    for (size_t i = data->key.data.list.n; i; --i, ++p) {
      freeKey(p);
    }
    free(data->key.data.list.root);
  }
}

static void freeKeys(DataQueryKey* arr, size_t sz, int doBreak) {
  for (; sz; --sz, ++arr) {
    if (doBreak && arr->type == DQNone)
      break;

    freeKey(arr);
  }
}

DataQueryKey copyKeyStructure(DataQueryKey data) {
  if (data.type != DQList) {
    switch (data.type) {
    case DQKeyword:
    case DQInt:
    case DQReal:
    case DQNone: {
      return data;
    }
    case DQString: {
      char* p = malloc(data.key.data.str.n+1);
      if (!p) {
        data.type = DQNone;
        return data;
      }
      memcpy(p, data.key.data.str.ptr, data.key.data.str.n);
      p[data.key.data.str.n] = 0;
      data.key.data.str.ptr = p;
      return data;
    }
    default:
      break;
    }
  }

  DataQueryKey r;
  r.type = DQList;
  r.key.data.list.n = 0;
  r.key.data.list.n = data.key.data.list.n;
  r.key.data.list.root = malloc(data.key.data.list.n * sizeof(DataQueryKey));
  if (!r.key.data.list.root) {
    r.type = DQNone;
    return r;
  }

  memset(r.key.data.list.root, 0, sizeof(DataQueryKey)*data.key.data.list.n);

  DataQueryKey* datap = data.key.data.list.root;
  DataQueryKey* dstp = r.key.data.list.root;
  for (size_t i = data.key.data.list.n; i; --i, ++datap, ++dstp) {
    DataQueryKey c = copyKeyStructure(*datap);
    if (c.type == DQNone) {
      freeKeys(r.key.data.list.root, data.key.data.list.n, 1);
      free(r.key.data.list.root);
      r.type = DQNone;
      return r; 
    }
    *dstp = c;
  }

  return r;
}

DataQueryKey createTreeStructure(StringTreeNode tree, ssize_t* hierarchy, size_t hchy_size) {
  while (hchy_size) {
    if (*hierarchy == -1) {
      /* vectorize */
      ++hierarchy;
      --hchy_size;

      DataQueryKey list;
      memset(&list, 0, sizeof(DataQueryKey));
      list.type = DQList;
      
      list.key.data.list.n = tree.nr_children;
      list.key.data.list.root = malloc(tree.nr_children * sizeof(DataQueryKey));
      if (!list.key.data.list.root) {
        list.type = DQNone;
        return list;
      }
      memset(list.key.data.list.root, 0, tree.nr_children * sizeof(DataQueryKey));

      for (size_t i = 0; i < tree.nr_children; ++i) {
        list.key.data.list.root[i] = createTreeStructure(tree.children[i], hierarchy, hchy_size);
        if (list.key.data.list.root[i].type == DQNone) {
          freeKeys(list.key.data.list.root, tree.nr_children, 1);
          list.type = DQNone;
          return list;
        }
      }

      return list;
    }

    tree = tree.children[*hierarchy];
    ++hierarchy;
    --hchy_size;
  }

  DataQueryKey key;
  key.type = DQString;
  size_t strln = strlen(tree.data);
  key.key.data.str.ptr = malloc(strln + 1);
  if (!key.key.data.str.ptr) {
    key.type = DQNone;
    return key;
  }
  strncpy(key.key.data.str.ptr, tree.data, strln);
  key.key.data.str.ptr[strln] = 0;
  key.key.data.str.n = strln;
  return key;
}

void data_query_exit(DataQueryKey *execStack, DataQueryKey *stackp, ssize_t *treeStack) {
  freeKeys(execStack, (stackp+1-execStack), 0);
  free(execStack);
  free(treeStack);
}

DataQueryKey miniml_data_query(DataQueryKey *arr, size_t size,
                               StringTreeNode tree) {
  DataQueryKey none;
  none.type = DQNone;

  /* exec stack */
  DataQueryKey *execStack = malloc(size * sizeof(DataQueryKey));
  if (!execStack) {
    return none;
  }
  DataQueryKey *stackp = execStack-1;
  size_t execStackCap = size;

  /* tree prop stack */
  ssize_t *treeStack = malloc(100 * sizeof(ssize_t));
  if (!treeStack) {
    free(execStack);
    return none;
  }
  ssize_t *treeptr = treeStack-1;
  size_t treeStackCap = 100;

  /* level 'state' */
  size_t lvl = 0;

  DataQueryKey *execp = arr;
  size_t i;
  for (i = size; i; --i, ++execp) {
    if (execp->type == DQKeyword) {

      /* clang-format off */

      switch (execp->key.word) {
      case DQK_toInt: {
        if (!(stackp+1-execStack)) { data_query_exit(execStack, stackp, treeStack); return none; }
        DataQueryKey r;
        int sccdd = convertToInt(&r, *stackp, lvl);
        if (!sccdd) { freeKey(&r); data_query_exit(execStack, stackp, treeStack); return none; }
        freeKey(stackp);
        *stackp = r;
        break;
      }
      case DQK_toFloat: {
        if (!(stackp+1-execStack)) { data_query_exit(execStack, stackp, treeStack); return none; }
        DataQueryKey r;
        int sccdd = convertToReal(&r, *stackp, lvl); /* changed this only */
        if (!sccdd) { freeKey(&r); data_query_exit(execStack, stackp, treeStack); return none; }
        freeKey(stackp);
        *stackp = r;
        break;
      }
      case DQK_toStr: {
        if (!(stackp+1-execStack)) { data_query_exit(execStack, stackp, treeStack); return none; }
        DataQueryKey r;
        int sccdd = convertToStr(&r, *stackp, lvl); /* changed this only */
        if (!sccdd) { freeKey(&r); data_query_exit(execStack, stackp, treeStack); return none; }
        freeKey(stackp);
        *stackp = r;
        break;
      }
      case DQK_propId: {
        /*assert*/
        if (!(stackp+1-execStack) || stackp->type != DQInt || stackp->key.data.integ < 0)
          { data_query_exit(execStack, stackp, treeStack); return none; }

        /*check capacity*/
        if ((treeptr+1-treeStack) == treeStackCap) {
          ssize_t* newTreeStack = realloc(treeStack, (treeStackCap + 100)*sizeof(size_t));
          if (!newTreeStack) { data_query_exit(execStack, stackp, treeStack); return none; }
          treeStack = newTreeStack;
          treeStackCap = treeStackCap + 100;
        }

        /*add prop*/
        ++treeptr;
        *treeptr = stackp->key.data.integ;
        --stackp;
        break;
      }
      case DQK_foreach: {
        if ((treeptr+1-treeStack) == treeStackCap) {
          ssize_t* newTreeStack = realloc(treeStack, (treeStackCap + 100)*sizeof(size_t));
          if (!newTreeStack) { data_query_exit(execStack, stackp, treeStack); return none; }
          treeStack = newTreeStack;
          treeStackCap = treeStackCap + 100;
        }
        ++treeptr;
        *treeptr = -1; /* 'for each' */
        break;
      }
      case DQK_changeLvl: {
        /*assert*/
        if (!(stackp+1-execStack) || stackp->type != DQInt || stackp->key.data.integ < 0)
          { data_query_exit(execStack, stackp, treeStack); return none; }

        lvl = stackp->key.data.integ;
        --stackp;
        break;
      }
      case DQK_up: {
        if ((treeptr+1-treeStack) != 0) { data_query_exit(execStack, stackp, treeStack); return none; }
        --treeptr;
        break;
      }
      case DQK_union: {
        if (!(stackp+1-execStack)) { data_query_exit(execStack, stackp, treeStack); return none; }
        DataQueryKey r;
        int sccdd = create_union(&r, *stackp, lvl);
        if (!sccdd) { freeKey(&r); data_query_exit(execStack, stackp, treeStack); return none; }
        freeKey(stackp);
        *stackp = r;
        break;
      }
      case DQK_multiplication: {
        if (!(stackp+1-execStack)) { data_query_exit(execStack, stackp, treeStack); return none; }

        DataQueryKey r;

        int sccdd = calculate_multiplication(&r, *stackp, lvl);
        if (!sccdd) { freeKey(&r); data_query_exit(execStack, stackp, treeStack); return none; }

        freeKey(stackp);
        *stackp = r;
        break;
      }
      case DQK_count: {
        if (!(stackp+1-execStack)) { data_query_exit(execStack, stackp, treeStack); return none; }
        DataQueryKey r;
        int sccdd = get_count(&r, *stackp, lvl);
        if (!sccdd) { freeKey(&r); data_query_exit(execStack, stackp, treeStack); return none; }
        freeKey(stackp);
        *stackp = r;
        break;
      }
      case DQK_avg: {
        if (!(stackp+1-execStack)) { data_query_exit(execStack, stackp, treeStack); return none; }
        DataQueryKey r;
        int sccdd = calculate_avg(&r, *stackp, lvl);
        if (!sccdd) { freeKey(&r); data_query_exit(execStack, stackp, treeStack); return none; }
        freeKey(stackp);
        *stackp = r;
        break;
      }
      case DQK_sum: {
        if (!(stackp+1-execStack)) { data_query_exit(execStack, stackp, treeStack); return none; }
        DataQueryKey r;
        int sccdd = calculate_sum(&r, *stackp, lvl);
        if (!sccdd) { freeKey(&r); data_query_exit(execStack, stackp, treeStack); return none; }
        freeKey(stackp);
        *stackp = r;
        break;
      }
      case DQK_min: {
        if (!(stackp+1-execStack)) { data_query_exit(execStack, stackp, treeStack); return none; }
        DataQueryKey r;
        int sccdd = calculate_min(&r, *stackp, lvl);
        if (!sccdd) { freeKey(&r); data_query_exit(execStack, stackp, treeStack); return none; }
        freeKey(stackp);
        *stackp = r;
        break;
      }
      case DQK_max: {
        if (!(stackp+1-execStack)) { data_query_exit(execStack, stackp, treeStack); return none; }
        DataQueryKey r;
        int sccdd = calculate_max(&r, *stackp, lvl);
        if (!sccdd) { freeKey(&r); data_query_exit(execStack, stackp, treeStack); return none; }
        freeKey(stackp);
        *stackp = r;
        break;
      }
      case DQK_fetch: {
        DataQueryKey newKey = createTreeStructure(tree, treeStack, (treeptr+1-treeStack));
        if (newKey.type == DQNone) { data_query_exit(execStack, stackp, treeStack); return none; }
        
        ++stackp;
        memcpy(stackp, &newKey, sizeof(DataQueryKey));
        break;
      }
      }

      /* clang-format on */

    } else {

      ++stackp;
      memcpy(stackp, execp, sizeof(DataQueryKey));
    }
  }

  /* if stack if empty */
  if ((stackp+1-execStack) == 0) {
    free(execStack);
    free(treeStack);
    return none;
  }

  /* if stack is not from a single element */
  if ((stackp+1-execStack) > 1) {
    freeKeys(execStack, (stackp+1-execStack), 0);
    free(execStack);
    free(treeStack);
    return none;
  }

  DataQueryKey result;
  memcpy(&result, execStack, sizeof(DataQueryKey));
  free(execStack);
  free(treeStack);

  return result;
}

/* dst already structured */
static int convertToInt(DataQueryKey* dst, DataQueryKey data, size_t lvl) {
  if (lvl) {
    DataQueryKey* datap = data.key.data.list.root;

    DataQueryKey* dstarr = calloc(data.key.data.list.n, sizeof(DataQueryKey));
    if (!dstarr) {
      return 0;
    }

    DataQueryKey* dstp = dstarr;
    for (size_t i = data.key.data.list.n; i; --i, ++datap, ++dstp) {
      if (!convertToInt(dstp, *datap, lvl-1)) {
        freeKeys(dstarr, data.key.data.list.n, 1);
        free(dstarr);
        return 0;
      }
    }

    dst->key.data.list.root = dstarr;
    dst->key.data.list.n = data.key.data.list.n;
    dst->type = DQList;
    return 1;
  }

  if (data.type == DQInt) {
    *dst = data;
    return 1;
  } else if (data.type == DQReal) {
    int_fast32_t result = (int_fast32_t)data.key.data.real;
    data.key.data.integ = result;
    data.type = DQInt;
    *dst = data;
    return 1;
  } else if (data.type == DQString) {
    char *end;
    errno = 0;
    int_fast32_t t = strtoll(data.key.data.str.ptr, &end, 10);
    if (data.key.data.str.ptr == end || errno == ERANGE) {
      data.type = DQNone;
      *dst = data;
      return 0;
    }
    data.key.data.integ = t;
    data.type = DQInt;
    *dst = data;
    return 1;
  }

  return 0;
}

static int convertToReal(DataQueryKey* dst, DataQueryKey data, size_t lvl) {
  if (lvl) {
    DataQueryKey* datap = data.key.data.list.root;

    DataQueryKey* dstarr = calloc(data.key.data.list.n, sizeof(DataQueryKey));
    if (!dstarr) {
      return 0;
    }

    DataQueryKey* dstp = dstarr;
    for (size_t i = data.key.data.list.n; i; --i, ++datap, ++dstp) {
      if (!convertToReal(dstp, *datap, lvl-1)) {
        freeKeys(dstarr, data.key.data.list.n, 1);
        free(dstarr);
        return 0;
      }
    }

    dst->key.data.list.root = dstarr;
    dst->key.data.list.n = data.key.data.list.n;
    dst->type = DQList;
    return 1;
  }

  if (data.type == DQReal) {
    *dst = data;
    return 1;
  } else if (data.type == DQInt) {
    double result = (double)data.key.data.integ;
    data.key.data.real = result;
    data.type = DQReal;
    *dst = data;
    return 1;
  } else if (data.type == DQString) {
    char *end;
    errno = 0;
    double t = strtod(data.key.data.str.ptr, &end);
    if (data.key.data.str.ptr == end || errno == ERANGE) {
      data.type = DQNone;
      *dst = data;
      return 0;
    }
    data.key.data.real = t;
    data.type = DQReal;
    *dst = data;
    return 1;
  }

  return 0;
}

static int convertToStr(DataQueryKey* dst, DataQueryKey data, size_t lvl) {
 if (lvl) {
    DataQueryKey* datap = data.key.data.list.root;

    DataQueryKey* dstarr = calloc(data.key.data.list.n, sizeof(DataQueryKey));
    if (!dstarr) {
      return 0;
    }

    DataQueryKey* dstp = dstarr;
    for (size_t i = data.key.data.list.n; i; --i, ++datap, ++dstp) {
      if (!convertToStr(dstp, *datap,  lvl-1)) {
        freeKeys(dstarr, data.key.data.list.n, 1);
        free(dstarr);
        return 0;
      }
    }

    dst->key.data.list.root = dstarr;
    dst->key.data.list.n = data.key.data.list.n;
    dst->type = DQList;
    return 1;
  }

  if (data.type == DQString) {
    if (1) {
      char* p = malloc(data.key.data.str.n+1);
      if (!p) {
        data.type = DQNone;
        *dst = data;
        return 0;
      }
      memcpy(p, data.key.data.str.ptr, data.key.data.str.n+1);
      p[data.key.data.str.n] = 0;
      data.key.data.str.ptr = p;
      *dst = data;
      return 1;
    }
    *dst = data;
    return 1;
  }

  if (data.type == DQInt) {
    int len = snprintf(NULL, 0, "%ld", data.key.data.integ);
    if (len == 0) {
      data.type = DQNone;
      *dst = data;
      return 0;
    }
    char* str = malloc(len + 1);
    if (!str) {
      data.type = DQNone;
      *dst = data;
      return 0;
    }
    snprintf(str, len + 1, "%ld", data.key.data.integ);
    data.key.data.str.ptr = str;
    data.key.data.str.n = len;
    data.type = DQString;
    *dst = data;
    return 1;
  }

  if (data.type == DQReal) {
    int len = snprintf(NULL, 0, "%lf", data.key.data.real);
    if (len == 0) {
      data.type = DQNone;
      *dst = data;
      return 0;
    }
    char* str = malloc(len + 1);
    if (!str) {
      data.type = DQNone;
      *dst = data;
      return 0;
    }
    snprintf(str, len + 1, "%lf", data.key.data.real);
    data.key.data.str.ptr = str;
    data.key.data.str.n = len;
    data.type = DQString;
    *dst = data;
    return 1;
  }

  if (data.type == DQList) {
    size_t n = data.key.data.list.n;
    DataQueryKey* arr = calloc(n, sizeof(DataQueryKey));
    if (!arr) {
      return 0;
    }
    for (size_t i = 0; i < n; ++i) {
      int scss = convertToStr(arr+i, data.key.data.list.root[i], 0);
      if (!scss) {
        freeKeys(arr, n, 1);
        free(arr);
        return 0;
      }
    }
    size_t len = 0;
    for (size_t i = 0; i < n; ++i) {
      len += arr[i].key.data.str.n + 3;
    }
    char* newStr = malloc(len);
    if (!newStr) {
      freeKeys(arr,n,0);
      free(arr);
      return 0;
    }

    size_t cc = 0;
    for (size_t i = 0; i < n; ++i) {
      newStr[cc] = '[';
      ++cc;
      memcpy(newStr+cc, arr[i].key.data.str.ptr, arr[i].key.data.str.n);
      cc += arr[i].key.data.str.n;
      newStr[cc] = ']';
      ++cc;
      newStr[cc] = ' ';
    }

    newStr[cc] = '\0';

    freeKeys(arr,n,0);
    free(arr);
    data.type = DQString;
    data.key.data.str.ptr = newStr;
    data.key.data.str.n = len;
    *dst = data;
    return 1;
  }

  return 0;
}

static int create_union(DataQueryKey* dst, DataQueryKey data, size_t lvl) {
  if (lvl) {
    DataQueryKey* datap = data.key.data.list.root;

    DataQueryKey* dstarr = calloc(data.key.data.list.n, sizeof(DataQueryKey));
    if (!dstarr) {
      return 0;
    }

    DataQueryKey* dstp = dstarr;
    for (size_t i = data.key.data.list.n; i; --i, ++datap, ++dstp) {
      if (!create_union(dstp, *datap, lvl-1)) {
        freeKeys(dstarr, data.key.data.list.n, 1);
        free(dstarr);
        return 0;
      }
    }

    dst->key.data.list.root = dstarr;
    dst->key.data.list.n = data.key.data.list.n;
    dst->type = DQList;
    return 1;
  }

  if (data.type != DQList) {
    return 0;
  }

  size_t n = 0;
  DataQueryKey* p = data.key.data.list.root;
  for (size_t i = data.key.data.list.n; i; --i, ++p) {
    n += p->key.data.list.n;
  }

  DataQueryKey* new_set = malloc(n * sizeof(DataQueryKey));
  if (!new_set) {
    return 0;
  }

  DataQueryKey* sp = new_set;
  p = data.key.data.list.root;
  for (size_t i = data.key.data.list.n; i; --i, ++p) {
    if (p->type != DQList) {
      freeKeys(new_set, n, 1);
      return 0;
    }
    DataQueryKey* p2 = p->key.data.list.root;
    for (size_t j = p->key.data.list.n; j; --j, ++p2) {
      *sp = copyKeyStructure(*p2);
      if (sp->type == DQNone) {
        freeKeys(new_set, n, 1);
        return 0;
      }
      ++sp;
    }
  }

  dst->key.data.list.root = new_set;
  dst->key.data.list.n = n;
  dst->type = DQList;
  return 1;
}

static int calculate_avg(DataQueryKey* dst, DataQueryKey data, size_t lvl) {
  if (lvl) {
    DataQueryKey* datap = data.key.data.list.root;

    DataQueryKey* dstarr = calloc(data.key.data.list.n, sizeof(DataQueryKey));
    if (!dstarr) {
      return 0;
    }

    DataQueryKey* dstp = dstarr;
    for (size_t i = data.key.data.list.n; i; --i, ++datap, ++dstp) {
      if (!calculate_avg(dstp, *datap, lvl-1)) {
        freeKeys(dstarr, data.key.data.list.n, 1);
        free(dstarr);
        return 0;
      }
    }

    dst->key.data.list.root = dstarr;
    dst->key.data.list.n = data.key.data.list.n;
    dst->type = DQList;
    return 1;
  }

  if (data.type != DQList || data.key.data.list.n == 0) {
    return 0;
  }

  DataQueryKey* p = data.key.data.list.root;
  DataQueryType type = data.key.data.list.root->type;

  switch (type) {
  case DQInt: {
    int_fast32_t sum = 0;
    for (size_t i = data.key.data.list.n; i; --i, ++p) {
      if (p->type != type) {
        return 0;
      }
      sum += p->key.data.integ;
    }

    dst->type = DQInt;
    dst->key.data.integ = sum / data.key.data.list.n;

    return 1;
  }
  case DQReal: {
    double sum = 0;
    for (size_t i = data.key.data.list.n; i; --i, ++p) {
      if (p->type != type) {
        return 0;
      }
      sum += p->key.data.real;
    }

    dst->type = DQReal;
    dst->key.data.real = sum / data.key.data.list.n;

    return 1;
  }
  default:
    return 0;
  }
  
  return 1;
}

static int calculate_sum(DataQueryKey* dst, DataQueryKey data, size_t lvl) {
  if (lvl) {
    DataQueryKey* datap = data.key.data.list.root;

    DataQueryKey* dstarr = calloc(data.key.data.list.n, sizeof(DataQueryKey));
    if (!dstarr) {
      return 0;
    }

    DataQueryKey* dstp = dstarr;
    for (size_t i = data.key.data.list.n; i; --i, ++datap, ++dstp) {
      if (!calculate_sum(dstp, *datap, lvl-1)) {
        freeKeys(dstarr, data.key.data.list.n, 1);
        free(dstarr);
        return 0;
      }
    }

    dst->key.data.list.root = dstarr;
    dst->key.data.list.n = data.key.data.list.n;
    dst->type = DQList;
    return 1;
  }

  if (data.type != DQList || data.key.data.list.n == 0) {
    return 0;
  }

  DataQueryKey* p = data.key.data.list.root;
  DataQueryType type = data.key.data.list.root->type;

  switch (type) {
  case DQInt: {
    int_fast32_t sum = 0;
    for (size_t i = data.key.data.list.n; i; --i, ++p) {
      if (p->type != type) {
        return 0;
      }
      sum += p->key.data.integ;
    }

    dst->type = DQInt;
    dst->key.data.integ = sum;

    return 1;
  }
  case DQReal: {
    double sum = 0;
    for (size_t i = data.key.data.list.n; i; --i, ++p) {
      if (p->type != type) {
        return 0;
      }
      sum += p->key.data.real;
    }

    dst->type = DQReal;
    dst->key.data.real = sum;

    return 1;
  }
  default:
    return 0;
  }
  
  return 1;
}

static int calculate_multiplication(DataQueryKey* dst, DataQueryKey data, size_t lvl) {
  if (lvl) {
    DataQueryKey* datap = data.key.data.list.root;

    DataQueryKey* dstarr = calloc(data.key.data.list.n, sizeof(DataQueryKey));
    if (!dstarr) {
      return 0;
    }

    DataQueryKey* dstp = dstarr;
    for (size_t i = data.key.data.list.n; i; --i, ++datap, ++dstp) {
      if (!calculate_multiplication(dstp, *datap, lvl-1)) {
        freeKeys(dstarr, data.key.data.list.n, 1);
        free(dstarr);
        return 0;
      }
    }

    dst->key.data.list.root = dstarr;
    dst->key.data.list.n = data.key.data.list.n;
    dst->type = DQList;
    return 1;
  }

  if (data.type != DQList || data.key.data.list.n == 0) {
    return 0;
  }

  DataQueryKey* p = data.key.data.list.root;
  DataQueryType type = data.key.data.list.root->type;

  switch (type) {
  case DQInt: {
    int_fast32_t mult = 1;
    for (size_t i = data.key.data.list.n; i; --i, ++p) {
      if (p->type != type) {
        return 0;
      }
      mult *= p->key.data.integ;
    }

    dst->type = DQInt;
    dst->key.data.integ = mult;

    return 1;
  }
  case DQReal: {
    double mult = 1;
    for (size_t i = data.key.data.list.n; i; --i, ++p) {
      if (p->type != type) {
        return 0;
      }
      mult *= p->key.data.real;
    }

    dst->type = DQReal;
    dst->key.data.real = mult;

    return 1;
  }
  default:
    return 0;
  }
  
  return 1;
}

static int calculate_min(DataQueryKey* dst, DataQueryKey data, size_t lvl) {
  if (lvl) {
    DataQueryKey* datap = data.key.data.list.root;

    DataQueryKey* dstarr = calloc(data.key.data.list.n, sizeof(DataQueryKey));
    if (!dstarr) {
      return 0;
    }

    DataQueryKey* dstp = dstarr;
    for (size_t i = data.key.data.list.n; i; --i, ++datap, ++dstp) {
      if (!calculate_min(dstp, *datap, lvl-1)) {
        freeKeys(dstarr, data.key.data.list.n, 1);
        free(dstarr);
        return 0;
      }
    }

    dst->key.data.list.root = dstarr;
    dst->key.data.list.n = data.key.data.list.n;
    dst->type = DQList;
    return 1;
  }

  if (data.type != DQList || data.key.data.list.n == 0) {
    return 0;
  }

  DataQueryKey* p = data.key.data.list.root;
  DataQueryType type = data.key.data.list.root->type;

  switch (type) {
  case DQInt: {
    int_fast32_t min = p->key.data.integ;
    for (size_t i = data.key.data.list.n; i; --i, ++p) {
      if (p->type != type) {
        return 0;
      }
      if (min > p->key.data.integ)
        min = p->key.data.integ;
    }

    dst->type = DQInt;
    dst->key.data.integ = min;

    return 1;
  }
  case DQReal: {
    double min = p->key.data.real;
    for (size_t i = data.key.data.list.n; i; --i, ++p) {
      if (p->type != type) {
        return 0;
      }
      if (min > p->key.data.real)
        min = p->key.data.real;
    }

    dst->type = DQReal;
    dst->key.data.real = min;

    return 1;
  }
  default:
    return 0;
  }
  
  return 1;
}

static int calculate_max(DataQueryKey* dst, DataQueryKey data, size_t lvl) {
  if (lvl) {
    DataQueryKey* datap = data.key.data.list.root;

    DataQueryKey* dstarr = calloc(data.key.data.list.n, sizeof(DataQueryKey));
    if (!dstarr) {
      return 0;
    }

    DataQueryKey* dstp = dstarr;
    for (size_t i = data.key.data.list.n; i; --i, ++datap, ++dstp) {
      if (!calculate_max(dstp, *datap, lvl-1)) {
        freeKeys(dstarr, data.key.data.list.n, 1);
        free(dstarr);
        return 0;
      }
    }

    dst->key.data.list.root = dstarr;
    dst->key.data.list.n = data.key.data.list.n;
    dst->type = DQList;
    return 1;
  }

  if (data.type != DQList || data.key.data.list.n == 0) {
    return 0;
  }

  DataQueryKey* p = data.key.data.list.root;
  DataQueryType type = data.key.data.list.root->type;

  switch (type) {
  case DQInt: {
    int_fast32_t max = p->key.data.integ;
    for (size_t i = data.key.data.list.n; i; --i, ++p) {
      if (p->type != type) {
        return 0;
      }
      if (max < p->key.data.integ)
        max = p->key.data.integ;
    }

    dst->type = DQInt;
    dst->key.data.integ = max;

    return 1;
  }
  case DQReal: {
    double max = p->key.data.real;
    for (size_t i = data.key.data.list.n; i; --i, ++p) {
      if (p->type != type) {
        return 0;
      }
      if (max < p->key.data.real)
        max = p->key.data.real;
    }

    dst->type = DQReal;
    dst->key.data.real = max;

    return 1;
  }
  default:
    return 0;
  }
  
  return 1;
}

static int get_count(DataQueryKey* dst, DataQueryKey data, size_t lvl) {
  if (lvl) {
    DataQueryKey* datap = data.key.data.list.root;

    DataQueryKey* dstarr = calloc(data.key.data.list.n, sizeof(DataQueryKey));
    if (!dstarr) {
      return 0;
    }

    DataQueryKey* dstp = dstarr;
    for (size_t i = data.key.data.list.n; i; --i, ++datap, ++dstp) {
      if (!get_count(dstp, *datap, lvl-1)) {
        freeKeys(dstarr, data.key.data.list.n, 1);
        free(dstarr);
        return 0;
      }
    }

    dst->key.data.list.root = dstarr;
    dst->key.data.list.n = data.key.data.list.n;
    dst->type = DQList;
    return 1;
  }

  if (data.type != DQList) {
    return 0;
  }

  dst->type = DQInt;
  dst->key.data.integ = data.key.data.list.n;
  return 1;
}
