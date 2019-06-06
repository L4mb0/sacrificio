const struct sched_class myfifo_sched_class = {
	.next			= &fair_sched_class,
	.enqueue_task		= enqueue_task_fifo,
	.dequeue_task		= dequeue_task_fifo,
	.yield_task		= yield_task_fifo,
	.yield_to_task		= yield_to_task_fifo,

	.check_preempt_curr	= check_preempt_curr_fifo,

	.pick_next_task		= pick_next_task_fifo,
	.put_prev_task		= put_prev_task_fifo,

#ifdef CONFIG_SMP
	.select_task_rq		= select_task_rq_fifo,
	.migrate_task_rq	= migrate_task_rq_fifo,

	.rq_online		= rq_online_fifo,
	.rq_offline		= rq_offline_fifo,

	.task_dead		= task_dead_fifo,
	.set_cpus_allowed	= set_cpus_allowed_common,
#endif

	.set_curr_task          = set_curr_task_fifo,
	.task_tick		= task_tick_fifo,
	.task_fork		= task_fork_fifo,

	.prio_changed		= prio_changed_fifo,
	.switched_from		= switched_from_fifo,
	.switched_to		= switched_to_fifo,

	.get_rr_interval	= get_rr_interval_fifo,

	.update_curr		= update_curr_fifo,

#ifdef CONFIG_fifo_GROUP_SCHED
	.task_change_group	= task_change_group_fifo,
#endif
};

static void put_prev_task_fifo(struct rq *rq, struct task_struct *prev){
    struct sched_entity *se = &prev->se;
	struct cfs_rq *cfs_rq;

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_of(se);
		put_prev_entity(cfs_rq, se);
	}
}

static struct task_struct *
pick_next_task_fifo(struct fifo_rq *fifo_rq, struct task_struct *prev, struct rq_flags *rf)
{
	struct sched_entity *se;
	struct task_struct *p;
	int new_tasks;

again:
	put_prev_task(rq, prev);

	do {
		se = pick_next_entity(fifo_rq, NULL);
		set_next_entity(fifo_rq, se); //TODO fix for fifo_rq
	    fifo_rq = group_cfs_rq(se);
	} while (cfs_rq);

	p = task_of(se);

done: __maybe_unused;

	if (hrtick_enabled(rq))
		hrtick_start_fair(rq, p);

	return p;

}

