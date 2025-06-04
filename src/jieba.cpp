#include "erl_nif.h"
#include "cppjieba/Jieba.hpp"

cppjieba::Jieba* segment = nullptr;

static char* term_to_cstring(ErlNifEnv* env, ERL_NIF_TERM term)
{
    ErlNifBinary bin;
    enif_inspect_binary(env, term, &bin);
    char* s = (char*)enif_alloc(bin.size + 1);
    memcpy(s, bin.data, bin.size);
    s[bin.size] = '\0';
    return s;
}

static ERL_NIF_TERM words_to_list(ErlNifEnv* env, const std::vector<std::string>& words)
{
    ERL_NIF_TERM r = enif_make_list(env, 0);
    ErlNifBinary h;
    size_t len;
    for(std::vector<std::string>::const_iterator i = words.begin(); i != words.end(); ++i) {
        len = i->size();
        enif_alloc_binary(len, &h);
        memcpy(h.data, i->c_str(), len);
        r = enif_make_list_cell(env, enif_make_binary(env, &h), r);
    }
    ERL_NIF_TERM result;
    enif_make_reverse_list(env, r, &result);
    return result;
}

static ERL_NIF_TERM tags_to_list(ErlNifEnv* env, const std::vector<std::pair<std::string, std::string>>& tags)
{
    ERL_NIF_TERM r = enif_make_list(env, 0);
    ErlNifBinary w;
    ErlNifBinary t;
    size_t len_w;
    size_t len_t;
    for(std::vector<std::pair<std::string, std::string>>::const_iterator i = tags.begin(); i != tags.end(); ++i) {
        len_w = i->first.size();
        enif_alloc_binary(len_w, &w);
        memcpy(w.data, i->first.c_str(), len_w);
        len_t = i->second.size();
        enif_alloc_binary(len_t, &t);
        memcpy(t.data, i->second.c_str(), len_t);
        r = enif_make_list_cell(env,
                               enif_make_tuple2(env, enif_make_binary(env, &w), enif_make_binary(env, &t)),
                               r);
    }
    ERL_NIF_TERM result;
    enif_make_reverse_list(env, r, &result);
    return result;
}

static char* list_to_cstring(ErlNifEnv* env, ERL_NIF_TERM term)
{
    unsigned int len;
    enif_get_list_length(env, term, &len);
    char* s = (char*)enif_alloc(++len);
    enif_get_string(env, term, s, len, ERL_NIF_LATIN1);
    return s;
}

#define SIMPLE_CUT(fname, method) \
static ERL_NIF_TERM fname(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])\
{\
    if(argc != 1)\
        return enif_make_badarg(env);\
    char* s = term_to_cstring(env, argv[0]);\
    std::vector<std::string> words;\
    if(segment)\
        segment->method(s, words);\
    ERL_NIF_TERM ret = words_to_list(env, words);\
    enif_free(s);\
    return ret;\
}

extern "C"{
SIMPLE_CUT(cut, Cut)
SIMPLE_CUT(cut_all, CutAll)
SIMPLE_CUT(cut_for_search, CutForSearch)
SIMPLE_CUT(cut_hmm, CutHMM)

static ERL_NIF_TERM cut_small(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    if(argc != 2)
        return enif_make_badarg(env);

    char* s = term_to_cstring(env, argv[0]);
    unsigned int max_word_len;
    enif_get_uint(env, argv[1], &max_word_len);

    std::vector<std::string> words;
    if(segment)
        segment->CutSmall(s, words, max_word_len);

    ERL_NIF_TERM result = words_to_list(env, words);
    enif_free(s);
    return result;
}

static ERL_NIF_TERM insert_user_word(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    if(argc != 1)
        return enif_make_badarg(env);

    char* s = term_to_cstring(env, argv[0]);

    bool ret = false;
    if(segment)
        ret = segment->InsertUserWord(s);

    enif_free(s);
    return enif_make_atom(env, ret ? "true" : "false");
}

static ERL_NIF_TERM reset_separators(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    if(argc != 1)
        return enif_make_badarg(env);

    char* s = term_to_cstring(env, argv[0]);

    if(segment)
        segment->ResetSeparators(s);

    enif_free(s);
    return enif_make_atom(env, "ok");
}

static ERL_NIF_TERM tag(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    if(argc != 1)
        return enif_make_badarg(env);

    char* s = term_to_cstring(env, argv[0]);
    std::vector<std::pair<std::string, std::string>> result;
    if(segment)
        segment->Tag(s, result);
    ERL_NIF_TERM ret = tags_to_list(env, result);
    enif_free(s);
    return ret;
}

static ERL_NIF_TERM lookup_tag(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    if(argc != 1)
        return enif_make_badarg(env);

    char* s = term_to_cstring(env, argv[0]);
    std::string tag_str;
    if(segment)
        tag_str = segment->LookupTag(s);
    ErlNifBinary b;
    enif_alloc_binary(tag_str.size(), &b);
    memcpy(b.data, tag_str.c_str(), tag_str.size());
    enif_free(s);
    return enif_make_binary(env, &b);
}

static ERL_NIF_TERM delete_user_word(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    if(argc != 1)
        return enif_make_badarg(env);

    char* s = term_to_cstring(env, argv[0]);
    bool ret = false;
    if(segment)
        ret = segment->DeleteUserWord(s);
    enif_free(s);
    return enif_make_atom(env, ret ? "true" : "false");
}

static ERL_NIF_TERM find(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    if(argc != 1)
        return enif_make_badarg(env);

    char* s = term_to_cstring(env, argv[0]);
    bool ret = false;
    if(segment)
        ret = segment->Find(s);
    enif_free(s);
    return enif_make_atom(env, ret ? "true" : "false");
}

static ERL_NIF_TERM load_dict(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    if(argc != 5)
        return enif_make_badarg(env);

    char* dict_path = list_to_cstring(env, argv[0]);
    char* model_path = list_to_cstring(env, argv[1]);
    char* user_dict_path = list_to_cstring(env, argv[2]);
    char* idf_path = list_to_cstring(env, argv[3]);
    char* stop_path = list_to_cstring(env, argv[4]);

    if(segment) {
        delete segment;
    }
    segment = new cppjieba::Jieba(dict_path, model_path, user_dict_path, idf_path, stop_path);
    enif_free(dict_path);
    enif_free(model_path);
    enif_free(user_dict_path);
    enif_free(idf_path);
    enif_free(stop_path);
    return enif_make_atom(env, "ok\0");
}

static ErlNifFunc nif_funcs[] =
{
    {"cut", 1, cut},
    {"cut_all", 1, cut_all},
    {"cut_for_search", 1, cut_for_search},
    {"cut_hmm", 1, cut_hmm},
    {"cut_small", 2, cut_small},
    {"insert_user_word", 1, insert_user_word},
    {"delete_user_word", 1, delete_user_word},
    {"find", 1, find},
    {"reset_separators", 1, reset_separators},
    {"tag", 1, tag},
    {"lookup_tag", 1, lookup_tag},
    {"load_dict", 5, load_dict}
};
}
ERL_NIF_INIT(Elixir.ExJieba.Jieba, nif_funcs, NULL, NULL, NULL, NULL)
