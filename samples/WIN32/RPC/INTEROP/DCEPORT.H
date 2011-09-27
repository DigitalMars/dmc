/*++

Copyright (c) 1993-1995 Microsoft Corporation

Module Name:

    dceport.h

Abstract:

    Include file defining types and macros which map DCE RPC APIs to
    Microsoft RPC APIs.  Useful when porting DCE RPC applications to MS RPC.

--*/

#ifndef DCEPORT_H
#define DCEPORT_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Define various idl types
*/
#define idl_char                unsigned char
#define idl_boolean             unsigned char
#define idl_byte                unsigned char
#define idl_small_int           char
#define idl_usmall_int          unsigned char
#define idl_short_int           signed short
#define idl_ushort_int          unsigned short
#define idl_long_int            long
#define idl_ulong_int           unsigned long
#define boolean32               unsigned long
#define unsigned32              unsigned long
#define unsigned16              unsigned short
#define idl_true                1
#define idl_false               0
#define unsigned_char_t         unsigned char
typedef unsigned char __RPC_FAR *unsigned_char_p_t;
typedef void __RPC_FAR          *idl_void_p_t;

#ifndef _ERROR_STATUS_T_DEFINED
typedef unsigned long error_status_t;
#define _ERROR_STATUS_T_DEFINED
#endif

/*
** Define various DCE RPC types
*/
#define rpc_if_handle_t             RPC_IF_HANDLE
#define rpc_ns_handle_t             RPC_NS_HANDLE
#define rpc_authz_handle_t          RPC_AUTHZ_HANDLE
#define rpc_auth_identity_handle_t  RPC_AUTH_IDENTITY_HANDLE
#define rpc_sm_thread_handle_t      RPC_SS_THREAD_HANDLE
#define rpc_mgr_epv_t               RPC_MGR_EPV __RPC_FAR *
#define rpc_object_inq_fn_t         RPC_OBJECT_INQ_FN __RPC_FAR *
#define rpc_auth_key_retrieval_fn_t RPC_AUTH_KEY_RETRIEVAL_FN
#define rpc_mgmt_authorization_fn_t RPC_MGMT_AUTHORIZATION_FN

/*
** Define rpc_binding_vector_t to match DCE
*/
#ifdef rpc_binding_vector_t
#undef rpc_binding_vector_t
#endif

typedef struct
{
     unsigned long           count;
     handle_t                binding_h[1];
} rpc_binding_vector_t, __RPC_FAR *rpc_binding_vector_p_t;

/*
** Define rpc_protseq_vector_t to match DCE
*/

typedef struct
{
    unsigned long           count;
    unsigned char *         protseq[1];
} rpc_protseq_vector_t, __RPC_FAR *rpc_protseq_vector_p_t;

/*
** Define rpc_stats_vector_t to match DCE
*/

typedef struct
{
    unsigned long           count;
    unsigned long           stats[1];
} rpc_stats_vector_t, __RPC_FAR *rpc_stats_vector_p_t;

/*
** Define uuid_t to match DCE
*/
#ifdef uuid_t
#undef uuid_t
#endif

typedef struct
{
    unsigned long            time_low;
    unsigned short           time_mid;
    unsigned short           time_hi_and_version;
    unsigned char            clock_seq_hi_and_reserved;
    unsigned char            clock_seq_low;
    unsigned char            node[6];
} uuid_t, __RPC_FAR *uuid_p_t;

/*
** Define uuid_vector_t to match DCE
*/
#ifdef uuid_vector_t
#undef uuid_vector_t
#endif

typedef struct
{
     unsigned long           count;
     uuid_p_t                uuid[1];
} uuid_vector_t, __RPC_FAR *uuid_vector_p_t;

/*
** Define rpc_if_id_t and rpc_if_id_p_t to match DCE
*/

typedef struct
{
    uuid_t                   uuid;
    unsigned short           vers_major;
    unsigned short           vers_minor;
} rpc_if_id_t, __RPC_FAR *rpc_if_id_p_t;

/*
** Define rpc_if_id_vector_t to match DCE
*/

typedef struct
{
    unsigned long           count;
    rpc_if_id_p_t           if_id[1];
} rpc_if_id_vector_t, __RPC_FAR *rpc_if_id_vector_p_t;

/*
** The MinThreads parameters to RpcServerListen()
** is not part of the DCE API rpc_server_listen().
** This is the default value.
*/

#define rpc_c_listen_min_threads_default 1

/*
** Define various constants
*/
#define rpc_c_ns_syntax_default            RPC_C_NS_SYNTAX_DEFAULT
#define rpc_c_ns_syntax_dce                RPC_C_SYNTAX_DCE
#define rpc_c_ns_default_exp_age           RPC_C_DEFAULT_EXP_AGE
#define rpc_c_protseq_max_reqs_default     RPC_C_PROTSEQ_MAX_REQS_DEFAULT
#define rpc_c_protseq_max_calls_default    RPC_C_PROTSEQ_MAX_REQS_DEFAULT
#define rpc_c_listen_max_calls_default     RPC_C_LISTEN_MAX_CALLS_DEFAULT
#define rpc_c_ep_all_elts                  RPC_C_EP_ALL_ELTS
#define rpc_c_ep_match_by_if               RPC_C_EP_MATCH_BY_IF
#define rpc_c_ep_match_by_obj              RPC_C_EP_MATCH_BY_OBJ
#define rpc_c_ep_match_by_both             RPC_C_EP_MATCH_BY_BOTH
#define rpc_c_vers_all                     RPC_C_VERS_ALL
#define rpc_c_vers_compatible              RPC_C_VERS_COMPATIBLE
#define rpc_c_vers_exact                   RPC_C_VERS_EXACT
#define rpc_c_vers_major_only              RPC_C_VERS_MAJOR_ONLY
#define rpc_c_vers_upto                    RPC_C_VERS_UPTO
#define rpc_c_profile_default_elt          RPC_C_PROFILE_DEFAULT_ELT
#define rpc_c_profile_all_elts             RPC_C_PROFILE_ALL_ELTS
#define rpc_c_profile_match_by_if          RPC_C_PROFILE_MATCH_BY_IF
#define rpc_c_profile_match_by_mbr         RPC_C_PROFILE_MATCH_BY_MBR
#define rpc_c_profile_match_by_both        RPC_C_PROFILE_MATCH_BY_BOTH
#define rpc_c_binding_min_timeout          RPC_C_BINDING_MIN_TIMEOUT
#define rpc_c_binding_default_timeout      RPC_C_BINDING_DEFAULT_TIMEOUT
#define rpc_c_binding_max_timeout          RPC_C_BINDING_MAX_TIMEOUT
#define rpc_c_binding_infinite_timeout     RPC_C_BINDING_INFINITE_TIMEOUT
#define rpc_c_stats_calls_in               RPC_C_STATS_CALLS_IN
#define rpc_c_stats_calls_out              RPC_C_STATS_CALLS_OUT
#define rpc_c_stats_pkts_in                RPC_C_STATS_PKTS_IN
#define rpc_c_stats_pkts_out               RPC_C_STATS_PKTS_OUT
#define rpc_c_mgmt_inq_if_ids              RPC_C_MGMT_INQ_IF_IDS
#define rpc_c_mgmt_inq_princ_name          RPC_C_MGMT_INQ_PRINC_NAME
#define rpc_c_mgmt_inq_stats               RPC_C_MGMT_INQ_STATS
#define rpc_c_mgmt_inq_server_listen       RPC_C_MGMT_INQ_SERVER_LISTEN
#define rpc_c_mgmt_stop_server_listen      RPC_C_MGMT_STOP_SERVER_LISTEN
#define rpc_c_mgmt_cancel_infinite_timeout RPC_C_CANCEL_INFINITE_TIMEOUT

/*
** Define DCE API equivalents
*/
#define rpc_binding_copy(source,dest,status) \
                *(status) = RpcBindingCopy(source,dest)

#define rpc_binding_free(binding,status) *(status) = RpcBindingFree(binding)

#define rpc_binding_from_string_binding(string_binding,binding,status) \
                *(status) = RpcBindingFromStringBinding(string_binding,binding)

#define rpc_binding_inq_auth_client(binding,privs,princ_name,protect_level, \
                authn_svc,authz_svc,status) \
                *(status) = RpcBindingInqAuthClient(binding,privs,princ_name, \
                protect_level,authn_svc,authz_svc)

#define rpc_binding_inq_auth_info(binding,princ_name,protect_level,\
                authn_svc,auth_identity,authz_svc,status) \
                *(status) = RpcBindingInqAuthInfo(binding,princ_name, \
                protect_level,authn_svc,auth_identity,authz_svc)

#define rpc_binding_inq_object(binding,object_uuid,status) \
                *(status) = RpcBindingInqObject(binding,\
                (UUID __RPC_FAR *)object_uuid)

#define rpc_binding_reset(binding,status) *(status) = RpcBindingReset(binding)

#define rpc_binding_server_from_client(cbinding,sbinding,status) \
                *(status) = RpcBindingServerFromClient(cbinding,sbinding)

#define rpc_binding_set_auth_info(binding,princ_name,protect_level,\
                authn_svc,auth_identity,authz_svc,status) \
                *(status) = RpcBindingSetAuthInfo(binding,princ_name,\
                protect_level,authn_svc,auth_identity,authz_svc)

#define rpc_binding_set_object(binding,object_uuid,status) \
                *(status) = RpcBindingSetObject(binding,\
                (UUID __RPC_FAR *)object_uuid)

#define rpc_binding_to_string_binding(binding,string_binding,status) \
                *(status) = RpcBindingToStringBinding(binding,string_binding)

#define rpc_binding_vector_free(binding_vector,status) \
                *(status) = RpcBindingVectorFree(\
                (RPC_BINDING_VECTOR __RPC_FAR * __RPC_FAR *)binding_vector)

#define rpc_ep_register(if_spec,binding_vec,object_uuid_vec,annotation,\
                status)\
                *(status) = RpcEpRegister(if_spec,\
                (RPC_BINDING_VECTOR __RPC_FAR *)binding_vec, \
                (UUID_VECTOR __RPC_FAR *)object_uuid_vec, annotation)

#define rpc_ep_register_no_replace(if_spec,binding_vec,object_uuid_vec,\
                annotation,status) \
                *(status) = RpcEpRegisterNoReplace(if_spec,\
                (RPC_BINDING_VECTOR __RPC_FAR *)binding_vec,\
                (UUID_VECTOR __RPC_FAR *)object_uuid_vec,annotation)

#define rpc_ep_resolve_binding(binding_h,if_spec,status) \
                *(status) = RpcEpResolveBinding(binding_h,if_spec)

#define rpc_ep_unregister(if_spec,binding_vec,object_uuid_vec,status) \
                *(status) = RpcEpUnregister(if_spec,\
                (RPC_BINDING_VECTOR __RPC_FAR *)binding_vec,\
                (UUID_VECTOR __RPC_FAR *)object_uuid_vec)

#define rpc_if_id_vector_free(if_id_vector,status) \
                *(status) = RpcIfIdVectorFree(\
                (RPC_IF_ID_VECTOR __RPC_FAR * __RPC_FAR *)if_id_vector)

#define rpc_if_inq_id(if_spec,if_id,status) \
                *(status) = RpcIfInqId(if_spec,(RPC_IF_ID __RPC_FAR *)if_id)

#define rpc_if_register_auth_info(if_spec,princ_name,protect_level,\
                authn_svc,auth_identity,authz_svc,status) \
                *(status) = RpcIfRegisterAuthInfo(if_spec,princ_name,\
                protect_level,authn_svc,auth_identity,authz_svc)

#define rpc_mgmt_ep_elt_inq_begin(ep_binding,inquiry_type,if_id,vers_option,\
                object_uuid,inquiry_context,status) \
                *(status) = RpcMgmtEpEltInqBegin(ep_binding,inquiry_type,if_id,\
                vers_option,object_uuid,inquiry_context)

#define rpc_mgmt_ep_elt_inq_done(inquiry_context,status) \
                *(status) = RpcMgmtEpEltInqDone(inquiry_context)

#define rpc_mgmt_ep_elt_inq_next(inquiry_context,if_id,binding,object_uuid,\
                annotation,status) \
                *(status) = RpcMgmtEpEltInqNext(inquiry_context,if_id,binding,\
                object_uuid,annotation)

#define rpc_mgmt_ep_unregister(ep_binding,if_id,binding,object_uuid,status) \
                *(status) = RpcMgmtEpUnregister(ep_binding,if_id,binding,\
                object_uuid)

#define rpc_mgmt_inq_com_timeout(binding,timeout,status) \
                *(status) = RpcMgmtInqComTimeout(binding,timeout)

#define rpc_mgmt_inq_dflt_protect_level(authn_svc,level,status) \
                *(status) = RpcMgmtInqDefaultProtectLevel(authn_svc,level)

#define rpc_mgmt_inq_if_ids(binding,if_id_vector,status) \
                *(status) = RpcMgmtInqIfIds((bindings),\
                (RPC_IF_ID_VECTOR __RPC_FAR * __RPC_FAR *)(if_id_vector))

#define rpc_mgmt_inq_server_princ_name(binding,authn_svc,princ_name,status) \
                *(status) = RpcMgmtInqServerPrincName(binding,authn_svc,\
                princ_name)

#define rpc_mgmt_inq_stats(binding,statistics,status) \
                *(status) = RpcMgmtInqStats(binding,\
                (RPC_STATS_VECTOR __RPC_FAR * __RPC_FAR *)statistics)

#define rpc_mgmt_is_server_listening(binding,status) \
                ( ((*(status) = RpcMgmtIsServerListening(binding)) == RPC_S_OK) \
                ? (1) : (*(status) == RPC_S_NOT_LISTENING) \
                ? (*(status) = RPS_S_OK, 0) : (0) )

#define rpc_mgmt_set_authorization_fn(authz_fn,status) \
                *(status) = RpcMgmtSetAuthorizathionFn(authz_fn)

#define rpc_mgmt_set_cancel_timeout(seconds,status) \
                *(status) = RpcMgmtSetCancelTimeout(seconds)

#define rpc_mgmt_set_com_timeout(binding,timeout,status) \
                *(status) = RpcMgmtSetComTimeout(binding,timeout)

#define rpc_mgmt_set_server_stack_size(size,status) \
                *(status) = RpcMgmtSetServerStackSize(size)

#define rpc_mgmt_stats_vector_free(stats,status) \
                *(status) = RpcMgmtStatsVectorFree(\
                (RPC_STATS_VECTOR __RPC_FAR * __RPC_FAR *)stats)

#define rpc_mgmt_stop_server_listening(binding,status) \
                *(status) = RpcMgmtStopServerListening(binding)

#define rpc_network_inq_protseqs(prots,status) \
                *(status) = RpcNetworkInqProtseqs(\
                (RPC_PROTSEQ_VECTOR __RPC_FAR * __RPC_FAR *)prots)

#define rpc_network_is_protseq_valid(protseq,status) \
                *(status) = RpcNetworkIsProtseqValid(protseq)

/*
** Define NSI equivalents
*/
#define rpc_ns_binding_export(name_syntax,entry_name,if_spec,\
                binding_vector, uuid_vector,status) \
                *(status) = RpcNsBindingExport(name_syntax,entry_name,\
                if_spec, (RPC_BINDING_VECTOR *)binding_vector,\
                (UUID_VECTOR __RPC_FAR *)uuid_vector)

#define rpc_ns_binding_import_begin(name_syntax,entry_name,if_spec,\
                object_uuid,import_context,status) \
                *(status) = RpcNsBindingImportBegin(name_syntax,entry_name,\
                if_spec,(UUID __RPC_FAR *)object_uuid,import_context)

#define rpc_ns_binding_import_done(import_context,status) \
                *(status) = RpcNsBindingImportDone(import_context)

#define rpc_ns_binding_import_next(import_context,binding,status) \
                *(status) = RpcNsBindingImportNext(import_context,binding)

#define rpc_ns_binding_inq_entry_name(binding,name_syntax,entry_name,status)\
                *(status) = RpcNsBindingInqEntryName(binding,name_syntax,\
                entry_name)

#define rpc_ns_binding_lookup_begin(name_syntax,entry_name,if_spec,\
                object_uuid,max_count,lookup_context,status) \
                *(status) = RpcNsBindingLookupBegin(name_syntax,entry_name,\
                if_spec,(UUID __RPC_FAR *)object_uuid,max_count,lookup_context)

#define rpc_ns_binding_lookup_done(lookup_context,status) \
                *(status) = RpcNsBindingLookupDone(lookup_context)

#define rpc_ns_binding_lookup_next(lookup_context,binding_vector,status) \
                *(status) = RpcNsBindingLookupNext(lookup_context, \
                (RPC_BINDING_VECTOR __RPC_FAR * __RPC_FAR *)binding_vector)

#define rpc_ns_binding_select(binding_vector,binding,status) \
                *(status) = RpcNsBindingSelect(\
                (RPC_BINDING_VECTOR __RPC_FAR *)binding_vector,binding)

#define rpc_ns_binding_unexport(name_syntax,entry_name,if_spec,\
                uuid_vector,status) \
                *(status) = RpcNsBindingUnexport(name_syntax,entry_name,\
                if_spec, (UUID_VECTOR __RPC_FAR *)uuid_vector)

#define rpc_ns_entry_expand_name(name_syntax,entry_name,expanded_name,\
                status)\
                *(status) = RpcNsEntryExpandName(name_syntax,entry_name,\
                expanded_name)

#define rpc_ns_entry_object_inq_begin(name_syntax,entry_name,\
                inquiry_context,status)\
                *(status) = RpcNsEntryObjectInqBegin(name_syntax,\
                entry_name,inquiry_context)

#define rpc_ns_entry_object_inq_done(inquiry_context,status) \
                *(status) = RpcNsEntryObjectInqDone(inquiry_context)

#define rpc_ns_entry_object_inq_next(inquiry_context,object_uuid,status) \
                *(status) = RpcNsEntryObjectInqNext(inquiry_context,\
                (UUID __RPC_FAR *)object_uuid)

#define rpc_ns_group_delete(name_syntax,group_name,status) \
                *(status) = RpcNsGroupDelete(name_syntax,group_name)

#define rpc_ns_group_mbr_add(name_syntax,group_name,member_name_syntax,\
                member_name,status) \
                *(status) = RpcNsGroupMbrAdd(name_syntax,group_name,\
                member_name_syntax,member_name)

#define rpc_ns_group_mbr_inq_begin(name_syntax,group_name,member_name_syntax,\
                inquiry_context,status) \
                *(status) = RpcNsGroupMbrInqBegin(name_syntax,group_name,\
                member_name_syntax,inquiry_context)

#define rpc_ns_group_mbr_inq_done(inquiry_context,status) \
                *(status) = RpcNsGroupMbrInqDone(inquiry_context)

#define rpc_ns_group_mbr_inq_next(inquiry_context,member_name,status) \
                *(status) = RpcNsGroupMbrInqNext(inquiry_context,member_name)

#define rpc_ns_group_mbr_remove(name_syntax,group_name,member_name_syntax,\
                member_name,status) \
                *(status) = RpcNsGroupMbrRemove(name_syntax,group_name,\
                member_name_syntax,member_name)

#define rpc_ns_mgmt_binding_unexport(name_syntax,entry_name,if_id,vers_option,\
                uuid_vector,status) \
                *(status) = RpcNsMgmtBindingUnexport(name_syntax,entry_name,\
                (RPC_IF_ID __RPC_FAR *)if_id,vers_option,\
                (UUID_VECTOR __RPC_FAR *)uuid_vector)

#define rpc_ns_mgmt_entry_create(name_syntax,entry_name,status) \
                *(status) = RpcNsMgmtEntryCreate(name_syntax,entry_name)

#define rpc_ns_mgmt_entry_delete(name_syntax,entry_name,status) \
                *(status) = RpcNsMgmtEntryDelete(name_syntax,entry_name)

#define rpc_ns_mgmt_entry_inq_if_ids(name_syntax,entry_name,if_id_vector,\
                status) \
                *(status) = RpcNsMgmtEntryInqIfIds(name_syntax,entry_name,\
                (RPC_IF_ID_VECTOR __RPC_FAR * __RPC_FAR *)if_id_vector)

#define rpc_ns_mgmt_handle_set_exp_age(ns_handle,expiration_age,status) \
                *(status) = RpcNsMgmtHandleSetExpAge(ns_handle,expiration_age)

#define rpc_ns_mgmt_inq_exp_age(expiration_age,status) \
                *(status) = RpcNsMgmtInqExpAge(expiration_age)

#define rpc_ns_mgmt_set_exp_age(expiration_age,status) \
                *(status) = RpcNsMgmtSetExpAge(expiration_age)

#define rpc_ns_profile_delete(name_syntax,profile_name,status) \
                *(status) = RpcNsProfileDelete(name_syntax,profile_name)

#define rpc_ns_profile_elt_add(name_syntax,profile_name,if_id,\
                member_name_syntax,member_name,priority,annotation,status) \
                *(status) = RpcNsProfileEltAdd(name_syntax,profile_name,\
                (RPC_IF_ID __RPC_FAR *)if_id,member_name_syntax,member_name,\
                priority,annotation)

#define rpc_ns_profile_elt_inq_begin(name_syntax,profile_name,inquiry_type,\
                if_id,if_vers_option,member_name_syntax,\
                member_name,inquiry_context,status) \
                *(status) = RpcNsProfileEltInqBegin(name_syntax,profile_name,\
                inquiry_type,(RPC_IF_ID __RPC_FAR *)if_id,if_vers_option,\
                member_name_syntax,member_name,inquiry_context)

#define rpc_ns_profile_elt_inq_done(inquiry_context,status) \
                *(status) = RpcNsProfileEltInqDone(inquiry_context)

#define rpc_ns_profile_elt_inq_next(inquiry_context,if_id,member_name,\
                priority,annotation,status) \
                *(status) = RpcNsProfileEltInqNext(inquiry_context,\
                (RPC_IF_ID __RPC_FAR *)if_id,member_name,priority,annotation)

#define rpc_ns_profile_elt_remove(name_syntax,profile_name,if_id,\
                member_name_syntax,member_name,status) \
                *(status) = RpcNsProfileEltRemove(name_syntax,profile_name,\
                (RPC_IF_ID __RPC_FAR *)if_id,member_name_syntax,member_name)

#define rpc_object_inq_type(object_uuid,type_uuid,status) \
                *(status) = RpcObjectInqType((UUID __RPC_FAR *)object_uuid,\
                (UUID __RPC_FAR *)type_uuid)

#define rpc_object_set_inq_fn(inq_fn,status) \
                *(status) = RpcObjectSetInqFn(inq_fn)

#define rpc_object_set_type(object_uuid,type_uuid,status) \
                *(status) = RpcObjectSetType((UUID __RPC_FAR *)object_uuid,\
                (UUID __RPC_FAR *)type_uuid)

#define rpc_protseq_vector_free(protseq_vector,status) \
                *(status) = RpcProtseqVectorFree( \
                (RPC_PROTSEQ_VECTOR __RPC_FAR * __RPC_FAR *)protseq_vector)

#define rpc_server_inq_bindings(binding_vector,status) \
                *(status) = RpcServerInqBindings(\
                (RPC_BINDING_VECTOR __RPC_FAR * __RPC_FAR *)binding_vector)

#define rpc_server_inq_if(if_spec,type_uuid,mgr_epv,status) \
                *(status) = RpcServerInqIf(if_spec,(UUID __RPC_FAR *)type_uuid,\
                (RPC_MGR_EPV __RPC_FAR *)mgr_epv)

#define rpc_server_listen(max_calls,status) \
                *(status) = RpcServerListen(rpc_c_listen_min_threads_default,\
                max_calls,0)

#define rpc_server_register_auth_info(princ_name,auth_svc,get_key_func,arg,\
                status) \
                *(status) = RpcServerRegisterAuthInfo(princ_name,auth_svc,\
                get_key_func,arg)

#define rpc_server_register_if(if_spec,type_uuid,mgr_epv,status) \
                *(status) = RpcServerRegisterIf(if_spec,\
                (UUID __RPC_FAR *)type_uuid,(RPC_MGR_EPV __RPC_FAR *)mgr_epv)

#define rpc_server_unregister_if(if_spec,type_uuid,status) \
                *(status) = RpcServerUnregisterIf(if_spec,(UUID *)type_uuid,0)

#define rpc_server_use_all_protseqs(max_call_requests,status) \
                *(status) = RpcServerUseAllProtseqs(max_call_requests,0)

#define rpc_server_use_all_protseqs_if(max_call_requests,if_spec,status) \
                *(status) = RpcServerUseAllProtseqsIf(max_call_requests,\
                if_spec,0)

#define rpc_server_use_protseq(protseq,max_call_requests,status) \
                *(status) = RpcServerUseProtseq(protseq,max_call_requests,0)

#define rpc_server_use_protseq_ep(protseq,max_call_requests,endpoint,status)\
                *(status) = RpcServerUseProtseqEp(protseq,max_call_requests,\
                endpoint,0)

#define rpc_server_use_protseq_if(protseq,max_call_requests,if_spec,status) \
                *(status) = RpcServerUseProtseqIf(protseq,max_call_requests,\
                if_spec,0)

#define rpc_sm_alloce(size,status) *(status) = RpcSmAllocate(size)

#define rpc_sm_client_free(ptr,status) *(status) = RpcSmClientFree(ptr)

#define rpc_sm_destroy_client_context(context,status) \
                *(status) = RpcSmDestroyClientContext(context)

#define rpc_sm_disable_allocate(status) *(status) = RpcSmDisableAllocate()

#define rpc_sm_enable_allocate(status) *(status) = RpcSmEnableAllocate()

#define rpc_sm_free(ptr,status) *(status) = RpcSmFree(ptr)

#define rpc_sm_get_thread_handle(status) RpcSmGetThreadHandle(status)

#define rpc_sm_set_client_alloc_free(alloc,free,status) \
                *(status) = RpcSmSetClientAllocFree(alloc,free)

#define rpc_sm_set_thread_handle(id,status) \
                *(status) = RpcSmSetThreadHandle(id)

#define rpc_sm_swap_client_alloc_free(alloc,free,old_alloc,old_free,status) \
                *(status) = RpcSmSwapClientAllocFree(alloc,free \
                old_alloc, old_free)

#define rpc_string_binding_compose(object_uuid,protseq,netaddr,endpoint,\
                options,binding,status) \
                *(status) = RpcStringBindingCompose(object_uuid,protseq,\
                netaddr,endpoint,options,binding)

#define rpc_string_binding_parse(string_binding,object_uuid,protseq,netaddr,\
                endpoint,options,status) \
                *(status) = RpcStringBindingParse(string_binding,\
                object_uuid,protseq,netaddr,endpoint,options)

#define rpc_string_free(string,status) *(status) = RpcStringFree(string)

#define uuid_compare(uuid1,uuid2,status) \
                UuidCompare((UUID __RPC_FAR *)(uuid1),\
                            (UUID __RPC_FAR *)(uuid2),(status))

#define uuid_create(uuid,status) \
                *(status) = UuidCreate((UUID __RPC_FAR *)uuid)

#define uuid_create_nil(uuid,status) \
                *(status) = UuidCreateNil((UUID __RPC_FAR *)uuid)

#define uuid_equal(uuid1,uuid2,status) \
                UuidEqual((UUID __RPC_FAR *)(uuid1), \
                          (UUID __RPC_FAR *)(uuid2), (status))

#define uuid_from_string(string,uuid,status) \
                *(status) = UuidFromString(string,(UUID __RPC_FAR *)uuid)

#define uuid_hash(uuid,status) \
                UuidHash((UUID __RPC_FAR *)(uuid),(status))

#define uuid_is_nil(uuid,status) \
                UuidIsNil((UUID __RPC_FAR *)(uuid), (status))

#define uuid_to_string(uuid,string,status)\
                *(status) = UuidToString((UUID __RPC_FAR *)uuid,string)


#define true  1
#define false 0

/*
** Define exception handling equivalents
**
*/
#if defined (__RPC_WIN16__) || defined (__RPC_DOS__)

#define TRY                                       \
    {                                                \
    int _exception_mode_finally;                     \
    int _exception_code;                             \
    ExceptionBuff exception;                         \
    _exception_code = RpcSetException(&exception);   \
    if (!_exception_code)                            \
    {

#define CATCH_ALL                                 \
        _exception_mode_finally = false;             \
        RpcLeaveException();                         \
        }                                            \
        else                                         \
        {
/*
 * #define CATCH(X)                               \
 *   }else if ((unsigned long)RpcExceptionCode()==(unsigned long)X) {
 */
#define FINALLY                                   \
        _exception_mode_finally = true;              \
        RpcLeaveException();                         \
        } {
#define ENDTRY                                    \
          }                                               \
        if (_exception_mode_finally && _exception_code)  \
            RpcRaiseException(_exception_code);           \
        }

#endif /* WIN16 or DOS */


#if defined (__RPC_WIN32__)
#define TRY             try {
/*
 * #define CATCH(X)        \
 *                      } except (GetExceptionCode() == X ? \
 *                        EXCEPTION_EXECUTE_HANDLER : \
 *                        EXCEPTION_CONTINUE_SEARCH) {
 */
#define CATCH_ALL       } except (EXCEPTION_EXECUTE_HANDLER) {
#define FINALLY         } finally {
#define ENDTRY          }
#endif /* WIN32 */

#define RAISE(v)   RpcRaiseException(v)
#define RERAISE    RpcRaiseException(RpcExceptionCode())
#define THIS_CATCH RpcExceptionCode()

/*
** DCE Status code mappings
*/
#ifndef rpc_s_ok
#define rpc_s_ok                        RPC_S_OK
#endif
#ifndef error_status_ok
#define error_status_ok                 RPC_S_OK
#endif
#define ept_s_cant_perform_op           EPT_S_CANT_PERFORM_OP
#define ept_s_invalid_entry             EPT_S_INVALID_ENTRY
#define ept_s_not_registered            EPT_S_NOT_REGISTERED
#define rpc_s_already_listening         RPC_S_ALREADY_LISTENING
#define rpc_s_already_registered        RPC_S_ALREADY_REGISTERED
#define rpc_s_binding_has_no_auth       RPC_S_BINDING_HAS_NO_AUTH
#define rpc_s_binding_imcomplete        RPC_S_BINDING_IMCOMPLETE
#define rpc_s_call_cancelled            RPC_S_CALL_CANCELLED
#define rpc_s_call_failed               RPC_S_CALL_FAILED
#define rpc_s_cant_bind_socket          RPC_S_CANNOT_BIND
#define rpc_s_cant_create_socket        RPC_S_CANT_CREATE_ENDPOINT
#define rpc_s_comm_failure              RPC_S_COMM_FAILURE
#define rpc_s_connect_no_resources      RPC_S_OUT_OF_RESOURCES
#define rpc_s_cthread_create_failed     RPC_S_OUT_OF_THREADS
#define rpc_s_endpoint_not_found        RPC_S_NO_ENDPOINT_FOUND
#define rpc_s_entry_already_exists      RPC_S_ENTRY_ALREADY_EXISTS
#define rpc_s_entry_not_found           RPC_S_ENTRY_NOT_FOUND
#define rpc_s_fault_addr_error          RPC_S_ADDRESS_ERROR
#define rpc_s_fault_fp_div_by_zero      RPC_S_FP_DIV_ZERO
#define rpc_s_fault_fp_overflow         RPC_S_FP_OVERFLOW
#define rpc_s_fault_fp_underflow        RPC_S_FP_UNDERFLOW
#define rpc_s_fault_int_div_by_zero     RPC_S_ZERO_DIVIDE
#define rpc_s_fault_invalid_bound       RPC_S_INVALID_BOUND
#define rpc_s_fault_invalid_tag         RPC_S_INVALID_TAG
#define rpc_s_fault_remote_no_memory    RPC_S_SERVER_OUT_OF_MEMORY
#define rpc_s_fault_unspec              RPC_S_CALL_FAILED
#define rpc_s_incomplete_name           RPC_S_INCOMPLETE_NAME
#define rpc_s_interface_not_found       RPC_S_INTERFACE_NOT_FOUND
#define rpc_s_internal_error            RPC_S_INTERNAL_ERROR
#define rpc_s_inval_net_addr            RPC_S_INVALID_NET_ADDR
#define rpc_s_invalid_arg               RPC_S_INVALID_ARG
#define rpc_s_invalid_binding           RPC_S_INVALID_BINDING
#define rpc_s_invalid_endpoint_format   RPC_S_INVALID_ENDPOINT_FORMAT
#define rpc_s_invalid_naf_id            RPC_S_INVALID_NAF_IF
#define rpc_s_invalid_name_syntax       RPC_S_INVALID_NAME_SYNTAX
#define rpc_s_invalid_rpc_protseq       RPC_S_INVALID_RPC_PROTSEQ
#define rpc_s_invalid_string_binding    RPC_S_INVALID_STRING_BINDING
#define rpc_s_invalid_timeout           RPC_S_INVALID_TIMEOUT
#define rpc_s_invalid_vers_option       RPC_S_INVALID_VERS_OPTION
#define rpc_s_max_calls_too_small       RPC_S_MAX_CALLS_TOO_SMALL
#define rpc_s_mgmt_op_disallowed        RPC_S_ACCESS_DENIED
#define rpc_s_name_service_unavailable  RPC_S_NAME_SERVICE_UNAVAILABLE
#define rpc_s_no_bindings               RPC_S_NO_BINDINGS
#define rpc_s_no_entry_name             RPC_S_NO_ENTRY_NAME
#define rpc_s_no_interfaces             RPC_S_NO_INTERFACES
#define rpc_s_no_interfaces_exported    RPC_S_NO_INTERFACES_EXPORTED
#define rpc_s_no_memory                 RPC_S_OUT_OF_MEMORY
#define rpc_s_no_more_elements          RPC_X_NO_MORE_ENTRIES
#define rpc_s_no_more_bindings          RPC_S_NO_MORE_BINDINGS
#define rpc_s_no_more_members           RPC_S_NO_MORE_MEMBERS
#define rpc_s_no_ns_permission          RPC_S_ACCESS_DENIED
#define rpc_s_no_princ_name             RPC_S_NO_PRINC_NAME
#define rpc_s_no_protseqs               RPC_S_NO_PROTSEQS
#define rpc_s_no_protseqs_registered    RPC_S_NO_PROTSEQS_REGISTERED
#define rpc_s_not_rpc_tower             RPC_S_CANNOT_SUPPORT
#define rpc_s_not_supported             RPC_S_CANNOT_SUPPORT
#define rpc_s_not_authorized            RPC_S_ACCESS_DENIED
#define rpc_s_nothing_to_unexport       RPC_S_NOTHING_TO_UNEXPORT
#define rpc_s_object_not_found          RPC_S_OBJECT_NOT_FOUND
#define rpc_s_protocol_error            RPC_S_PROTOCOL_ERROR
#define rpc_s_protseq_not_supported     RPC_S_PROTSEQ_NOT_SUPPORTED
#define rpc_s_server_too_busy           RPC_S_SERVER_TOO_BUSY
#define rpc_s_string_too_long           RPC_S_STRING_TOO_LONG
#define rpc_s_type_already_registered   RPC_S_TYPE_ALREADY_REGISTERED
#define rpc_s_unknown_authn_service     RPC_S_UNKNOWN_AUTHN_SERVICE
#define rpc_s_unknown_authz_service     RPC_S_UNKNOWN_AUTHZ_SERVICE
#define rpc_s_unknown_if                RPC_S_UNKNOWN_IF
#define rpc_s_unknown_mgr_type          RPC_S_UNKNOWN_MGR_TYPE
#define rpc_s_unknown_reject            RPC_S_CALL_FAILED_DNE
#define rpc_s_unsupported_name_syntax   RPC_S_UNSUPPORTED_NAME_SYNTAX
#define rpc_s_unsupported_type          RPC_S_UNSUPPORTED_TYPE
#define rpc_s_wrong_boot_time           RPC_S_CALL_FAILED_DNE
#define rpc_s_wrong_kind_of_binding     RPC_S_WRONG_KIND_OF_BINDING
#define uuid_s_ok                       RPC_S_OK
#define uuid_s_internal_error           RPC_S_INTERNAL_ERROR
#define uuid_s_invalid_string_uuid      RPC_S_INVALID_STRING_UUID
#define uuid_s_no_address               RPC_S_UUID_NO_ADDRESS

/*
** DCE Exception mappings
*/

#define rpc_x_comm_failure              RPC_S_COMM_FAILURE
#define rpc_x_connect_no_resources      RPC_S_OUT_OF_RESOURCES
#define rpc_x_entry_not_found           RPC_S_ENTRY_NOT_FOUND
#define rpc_x_incomplete_name           RPC_S_INCOMPLETE_NAME
#define rpc_x_invalid_arg               RPC_S_INVALID_ARG
#define rpc_x_invalid_binding           RPC_S_INVALID_BINDING
#define rpc_x_invalid_bound             RPC_X_INVALID_BOUND
#define rpc_x_invalid_endpoint_format   RPC_S_INVALID_ENDPOINT_FORMAT
#define rpc_x_invalid_naf_id            RPC_S_INVALID_NAF_IF
#define rpc_x_invalid_name_syntax       RPC_S_INVALID_NAME_SYNTAX
#define rpc_x_invalid_rpc_protseq       RPC_S_INVALID_RPC_PROTSEQ
#define rpc_x_invalid_tag               RPC_X_INVALID_TAG
#define rpc_x_invalid_timeout           RPC_S_INVALID_TIMEOUT
#define rpc_x_no_memory                 RPC_X_NO_MEMORY
#define rpc_x_object_not_found          RPC_S_OBJECT_NOT_FOUND
#define rpc_x_protocol_error            RPC_S_PROTOCOL_ERROR
#define rpc_x_protseq_not_supported     RPC_S_PROTSEQ_NOT_SUPPORTED
#define rpc_x_server_too_busy           RPC_S_SERVER_TOO_BUSY
#define rpc_x_ss_char_trans_open_fail   RPC_X_SS_CHAR_TRANS_OPEN_FAIL
#define rpc_x_ss_char_trans_short_file  RPC_X_SS_CHAR_TRANS_SHORT_FILE
#define rpc_x_ss_context_damaged        RPC_X_SS_CONTEXT_DAMAGED
#define rpc_x_ss_context_mismatch       RPC_X_SS_CONTEXT_MISMATCH
#define rpc_x_ss_in_null_context        RPC_X_SS_IN_NULL_CONTEXT
#define rpc_x_string_too_long           RPC_S_STRING_TOO_LONG
#define rpc_x_unknown_if                RPC_S_UNKNOWN_IF
#define rpc_x_unknown_mgr_type          RPC_S_UNKNOWN_MGR_TYPE
#define rpc_x_unsupported_name_syntax   RPC_S_UNSUPPORTED_NAME_SYNTAX
#define rpc_x_unsupported_type          RPC_S_UNSUPPORTED_TYPE
#define rpc_x_wrong_boot_time           RPC_S_CALL_FAILED_DNE
#define rpc_x_wrong_kind_of_binding     RPC_S_WRONG_KIND_OF_BINDING
#define uuid_x_internal_error           RPC_S_INTERNAL_ERROR

#ifdef __cplusplus
}
#endif

#endif /* DCEPORT_H */

