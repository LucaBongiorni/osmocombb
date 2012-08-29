#include <osmocom/bb/common/osmocom_data.h>
#include <osmocom/bb/mobile/vty.h>

#if 0
static float time_diff(struct timeval *t1, struct timeval *t2)
{
        float delta;

        delta = (t2->tv_sec - t1->tv_sec);
        delta += 0.000001 * (t2->tv_usec - t1->tv_usec);

        return delta;
}

static void timeout_tcatcher(void *arg)
{
        struct osmocom_ms *ms = arg;
        struct catcher_status *cs = &ms->catch_stat;
	float delta;

        gettimeofday(&cs->stop_time, 0);

	delta = time_diff(&cs->start_time, &cs->stop_time);

	vty_notify(ms, "delta %f current %d\n", delta, cs->current);

	switch(cs->current) {
	case CATCH_IDLE:
		return;
	case CATCH_TRY:
		if (delta > 2.0) {
			if (cs->flag < 2)
				cs->flag = 2;
		}
		break;
	case CATCH_EST:
		if (delta > 2.0) {
			if (cs->flag < 2)
				cs->flag = 2;
		}
		if (delta > 10.0) {
			if (cs->flag < 3)
				cs->flag = 3;
		}
		break;
	case CATCH_DATA:
		if (delta > 2.0) {
			if (cs->flag < 1)
				cs->flag = 1;
		}
		if (delta > 10.0) {
			if (cs->flag < 2)
				cs->flag = 2;
		}
		break;
	case CATCH_SERV:
		return;
	}

        osmo_timer_schedule(&cs->dcch_time, 0, 100000);
}

void start_tcatcher(struct osmocom_ms *ms)
{
        struct catcher_status *cs = &ms->catch_stat;

        cs->dcch_time.cb = timeout_tcatcher;
        cs->dcch_time.data = ms;

        gettimeofday(&ms->catch_stat.start_time, 0);

        osmo_timer_schedule(&cs->dcch_time, 0, 100000);
}

void stop_tcatcher(struct osmocom_ms *ms)
{
        struct catcher_status *cs = &ms->catch_stat;

        if (osmo_timer_pending(&cs->dcch_time)) {
                osmo_timer_del(&cs->dcch_time);
        }
}

#endif
void imsi_finder_paging(struct osmocom_ms *ms, struct msgb *msg)
{
	uint8_t *mi;
        char imsi[16];
        uint32_t tmsi;

	mi = msg->data;

	if (mi[0] < 1)
		return;

	mi_type = mi[1] & GSM_MI_TYPE_MASK;

	switch (mi_type) {
	case GSM_MI_TYPE_TMSI:
		if (mi[0] < 5)
			return;
		memcpy(&tmsi, mi+2, 4);
		LOGP(DPAG, LOGL_INFO, "FELIPE TMSI %08x (not for us)\n", ntohl(tmsi));
		break;
	case GSM_MI_TYPE_IMSI:
		gsm48_mi_to_string(imsi, sizeof(imsi), mi + 1, mi[0]);
		LOGP(DPAG, LOGL_INFO, "FELIPE IMSI %s (not for us)\n", imsi);
		break;
	default:
		LOGP(DPAG, LOGL_NOTICE, "FELIPE Paging with unsupported MI type %d.\n", mi_type);
	}

}

void start_imsi_finder(struct osmocom_ms *ms)
{
	ms.imsi_finder.on = 1;
}

void stop_imsi_finder(struct osmocom_ms *ms)
{
	ms.imsi_finder.on = 0;
}
