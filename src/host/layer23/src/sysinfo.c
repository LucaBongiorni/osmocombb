/*
 * (C) 2010 by Andreas Eversberg <jolly@eversberg.eu>
 *
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <osmocom/osmocom_data.h>
#include <osmocom/networks.h>

int gsm48_sysinfo_dump(struct osmocom_ms *ms, struct gsm48_sysinfo *s)
{
	char buffer[80];
	int i, j;

	/* available sysinfos */
	printf("Available SYSTEM INFORMATIONS:");
	if (s->si1)
		printf(" 1");
	if (s->si2)
		printf(" 2");
	if (s->si2bis)
		printf(" 2bis");
	if (s->si2ter)
		printf(" 2ter");
	if (s->si3)
		printf(" 3");
	if (s->si4)
		printf(" 4");
	if (s->si5)
		printf(" 5");
	if (s->si5bis)
		printf(" 5bis");
	if (s->si5ter)
		printf(" 5ter");
	if (s->si6)
		printf(" 6");
	printf("\n\n");

	/* frequency map */
	for (i = 0; i < 1024; i += 64) {
		if (i < 10)
			sprintf(buffer, "   %d ", i);
		else if (i < 100)
			sprintf(buffer, "  %d ", i);
		else
			sprintf(buffer, " %d ", i);
		for (j = 0; j < 64; j++) {
			if ((s->freq[i+j].mask & FREQ_TYPE_SERV))
				buffer[j + 5] = 'S';
			else if ((s->freq[i+j].mask & FREQ_TYPE_HOPP))
				buffer[j + 5] = 'H';
			else if ((s->freq[i+j].mask & FREQ_TYPE_NCELL_2))
				buffer[j + 5] = 'N';
			else if ((s->freq[i+j].mask & FREQ_TYPE_NCELL_2bis))
				buffer[j + 5] = 'b';
			else if ((s->freq[i+j].mask & FREQ_TYPE_NCELL_2ter))
				buffer[j + 5] = 't';
			else if ((s->freq[i+j].mask & FREQ_TYPE_REP_5))
				buffer[j + 5] = 'R';
			else if ((s->freq[i+j].mask & FREQ_TYPE_REP_5bis))
				buffer[j + 5] = 'b';
			else if ((s->freq[i+j].mask & FREQ_TYPE_REP_5ter))
				buffer[j + 5] = 't';
			else
				buffer[j + 5] = '.';
		}
		sprintf(buffer + 69, " %d", i + 63);
		printf("%s\n", buffer);
	}
	printf(" S = serv. cell  H = hopping seq.  N,b,t = neigh. cells  "
		"R,b,t = cells to rep.\n\n");

	/* serving cell */
	printf("Serving Cell:\n");
	printf(" MCC = %03d  MNC = %02d  LAC = 0x%04x  Cell ID = 0x%04x  "
		"(%s, %s)\n", s->mcc, s->mnc, s->lac, s->cell_id,
		gsm_get_mcc(s->mcc), gsm_get_mnc(s->mcc, s->mnc));
	printf(" MAX_RETRANS = %d  TX_INTEGER = %d  re-establish = %s\n",
		s->max_retrans, s->tx_integer,
		(s->reest_denied) ? "denied" : "allowed");
	printf(" Cell barred = %s  barred classes =",
		(s->cell_barr ? "yes" : "no"));
	for (i = 0; i < 16; i++) {
		if ((s->class_barr & (1 << i)))
			printf(" C%d", i);
	}
	printf("\n\n");

	/* neighbor cell */
	printf("Neighbor Cell:\n");
	printf(" MAX_RETRANS = %d  TX_INTEGER = %d  re-establish = %s\n",
		s->nb_max_retrans, s->nb_tx_integer,
		(s->nb_reest_denied) ? "denied" : "allowed");
	printf(" Cell barred = %s  barred classes =",
		(s->nb_cell_barr ? "yes" : "no"));
	for (i = 0; i < 16; i++) {
		if ((s->nb_class_barr & (1 << i)))
			printf(" C%d", i);
	}
	printf("\n\n");

	/* cell selection */
	printf("MX_TXPWR_MAX_CCCH = %d  CRH = %d  RXLEV_MIN = %d  NECI = %d  "
		"ACS = %d\n", s->ms_txpwr_max_ccch, s->cell_resel_hyst_db,
		s->rxlev_acc_min_db, s->neci, s->acs);

	/* bcch options */
	printf("BCCH link timeout = %d  DTX = %d  PWRC = %d\n",
		s->bcch_radio_link_timeout, s->bcch_dtx, s->bcch_pwrc);

	/* sacch options */
	printf("SACCH link timeout = %d  DTX = %d  PWRC = %d\n",
		s->sacch_radio_link_timeout, s->sacch_dtx, s->sacch_pwrc);

	/* control channel */
	switch(s->ccch_conf) {
	case 0:
		printf("CCCH Config = 1 CCCH");
		break;
	case 1:
		printf("CCCH Config = 1 CCCH + SDCCH");
		break;
	case 2:
		printf("CCCH Config = 2 CCCH");
		break;
	case 4:
		printf("CCCH Config = 3 CCCH");
		break;
	case 6:
		printf("CCCH Config = 4 CCCH");
		break;
	default:
		printf("CCCH Config = reserved");
	}
	printf("BS-PA-MFMS = %d  Attachment = %s\n",
		s->pag_mf_periods, (s->att_allowed) ? "allowed" : "denied");
	printf("BS-AG_BLKS_RES = %d\n", s->bs_ag_blks_res);

	/* channel description */
	if (s->h)
		printf("chan_nr = 0x%02x TSC = %d  MAIO = %d  HSN = %d\n",
			s->chan_nr, s->tsc, s->maio, s->hsn);
	else
		printf("chan_nr = 0x%02x TSC = %d  ARFCN = %d\n",
			s->chan_nr, s->tsc, s->arfcn);
	printf("\n");

	return 0;
}
